/*
 * $Id: fst_download.c,v 1.23 2004/03/08 21:09:57 mkern Exp $
 *
 * Copyright (C) 2003 giFT-FastTrack project
 * http://developer.berlios.de/projects/gift-fasttrack
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include "fst_fasttrack.h"
#include "fst_download.h"

/*****************************************************************************/

static int download_client_callback (FSTHttpClient *client,
									 FSTHttpClientCbCode code);
static void download_write_gift (Source *source, unsigned char *data,
								 unsigned int len);
static void download_error_gift (Source *source, int remove_source,
								 unsigned short klass, char *error);
static char *download_calc_xferuid (char *uri);

/*****************************************************************************/

/* called by gift to start downloading of a chunk */
int fst_giftcb_download_start (Protocol *p, Transfer *transfer, Chunk *chunk,
							   Source *source)
{
	in_addr_t ip;
	in_port_t port;
	FSTHash *hash;

	/* determine whether we need to send a push. */
	if (!(hash = fst_download_parse_url (source->url, &ip, &port, NULL)))
	{
		FST_WARN_1 ("malformed url %s", source->url);
		return FALSE;
	}
	fst_hash_free (hash);

	if (fst_utils_ip_private (ip) || !port)
	{
		/* firewalled source, send push request */
		FSTPush *push;

		/* if we don't wait for push replies there is no point in requesting */
		if (!FST_PLUGIN->server)
			return FALSE;

		if ((push = fst_pushlist_lookup_source (FST_PLUGIN->pushlist, source)))
		{
			/* We already sent a push request for this source, remove it.
			 * Actually we shouldn't even get here since we removed the push in
			 * fst_giftcb_download_stop
			 */
			FST_WARN_2 ("removing old push for %s with id %d",
						source->url, push->push_id);
			fst_pushlist_remove (FST_PLUGIN->pushlist, push);
			fst_push_free (push);
		}

		if (! (push = fst_pushlist_add (FST_PLUGIN->pushlist, source)))
			return FALSE;

		if (!fst_push_send_request (push, FST_PLUGIN->session))
		{
			FST_DBG_1 ("fst_push_send_request failed for %s", source->url);
			FST_PROTO->source_status (FST_PROTO, source, SOURCE_TIMEOUT,
									  "Can't send push yet");
			fst_pushlist_remove (FST_PLUGIN->pushlist, push);
			fst_push_free (push);
			return FALSE;
		}

		FST_PROTO->source_status (FST_PROTO, source, SOURCE_WAITING, "Sent push");
		return TRUE;
	}

	if (!fst_download_start (source, NULL))
	{
		FST_DBG ("fst_download_start failed");
		return FALSE;
	}

	return TRUE;
}

/* called by gift to stop download */
void fst_giftcb_download_stop (Protocol *p, Transfer *transfer, Chunk *chunk,
							   Source *source, int complete)
{
	FSTHttpClient *client = (FSTHttpClient*) source->udata;
	FSTPush *push;

	/* close connection if there is outstanding data */
	if (client && client->state != HTCL_CONNECTED)
	{
		fst_http_client_cancel (client);
		FST_HEAVY_DBG_1 ("request cancelled for url %s", source->url);
	}

	/* remove push */
	if ((push = fst_pushlist_lookup_source (FST_PLUGIN->pushlist, source)))
	{
		FST_HEAVY_DBG_2 ("removing push for %s with id %d",
						 source->url, push->push_id);
		fst_pushlist_remove (FST_PLUGIN->pushlist, push);
		fst_push_free (push);
	}
}

/* called by gift to add a source */
BOOL fst_giftcb_source_add (Protocol *p,Transfer *transfer, Source *source)
{
	source->udata = NULL;

	return TRUE;
}

/* called by gift to remove source */
void fst_giftcb_source_remove (Protocol *p, Transfer *transfer,
							   Source *source)
{
	FSTHttpClient *client = (FSTHttpClient*) source->udata;
	FSTPush *push;

	if (client)
	{
		FST_HEAVY_DBG_1 ("removing source %s", source->url);
		fst_http_client_free (client);
		source->udata = NULL;
	}

	/* remove push */
	if ((push = fst_pushlist_lookup_source (FST_PLUGIN->pushlist, source)))
	{
		FST_HEAVY_DBG_2 ("removing push for %s with id %d",
						 source->url, push->push_id);
		fst_pushlist_remove (FST_PLUGIN->pushlist, push);
		fst_push_free (push);
	}
}

/*****************************************************************************/

/* start download for source, optionally using existing tcpcon */
int fst_download_start (Source *source, TCPC *tcpcon)
{
	Chunk *chunk = source->chunk;
	FSTHttpClient *client = (FSTHttpClient*) source->udata;
	FSTHttpHeader *request;
	in_addr_t ip;
	in_port_t port;
	char *uri, *range_str;
	FSTHash *hash;

	assert (source);
	assert (chunk);

	/* parse url */
	if (!(hash = fst_download_parse_url (source->url, &ip, &port, NULL)))
	{
		FST_WARN_1 ("malformed url %s", source->url);
		return FALSE;
	}

	uri = stringf_dup ("/.hash=%s", fst_hash_encode16_fthash (hash));
	fst_hash_free (hash);

	/* create http request */
	if (! (request = fst_http_header_request (HTHD_VER_11, HTHD_GET, uri)))
	{
		FST_WARN_1 ("creation of request failed for url %s", source->url);
		return FALSE;
	}

	/* remove old client and create new one if pushed connection */
	if (tcpcon)
	{
		FST_HEAVY_DBG_1 ("creating client for pushed connection from source %s",
						 source->url);

		fst_http_client_free (client);
		client = fst_http_client_create_tcpc (tcpcon, download_client_callback);

		client->udata = (void*) source;
		source->udata = (void*) client;
	}
	else if (!client)
	{
		FST_HEAVY_DBG_1 ("first time use of source %s", source->url);

		client = fst_http_client_create (net_ip_str (ip), port,
										 download_client_callback);
		client->udata = (void*) source;
		source->udata = (void*) client;
	}

	/* add some http headers */
	fst_http_header_set_field (request, "UserAgent", FST_HTTP_AGENT);
	fst_http_header_set_field (request, "X-Kazaa-Network", FST_NETWORK_NAME);
	fst_http_header_set_field (request, "X-Kazaa-Username", FST_USER_NAME);

	/* http range is inclusive!
	 * use chunk->start + chunk->transmit for starting point, rather non-intuitive
	 */
	range_str = stringf ("bytes=%d-%d", (int)(chunk->start + chunk->transmit),
						 (int)chunk->stop - 1);
	fst_http_header_set_field (request, "Range", range_str);

#ifdef FST_DOWNLOAD_BOOST_PL
	fst_http_header_set_field (request, "X-Kazaa-XferUid",
							   download_calc_xferuid (uri));
#endif

	free (uri);

	/* make the request */
	FST_PROTO->source_status (FST_PROTO, source, SOURCE_WAITING, "Connecting");

	if (! fst_http_client_request (client, request, FALSE))
	{
		FST_WARN_1 ("request failed for url %s", source->url);
		return FALSE;
	}

	return TRUE;
}

/* Parses new format url.
 * Returns FSTHash which caller frees or NULL on failure.
 * params receives a dataset with additional params, caller frees, may be NULL
 */
FSTHash *fst_download_parse_url (char *url, in_addr_t *ip,
                                 in_port_t *port, Dataset **params)
{
	char *url0, *hash_str, *param_str;
	char *ip_str, *port_str;
	FSTHash *hash;

	if (!url)
		return NULL;

	url0 = param_str = strdup (url);

	string_sep (&param_str, "://");

	/* separate ip and port */
	if (! (port_str = string_sep (&param_str, "/")))
	{
		free (url0);
		return NULL;
	}

	if (! (ip_str = string_sep (&port_str, ":")))
	{
		free (url0);
		return NULL;
	}

	if (ip) *ip = net_ip (ip_str);
	if (port) *port = ATOI (port_str);

	/* decode hash */
	if (! (hash_str = string_sep (&param_str, "?")))
		hash_str = param_str;

	hash = fst_hash_create ();
	if (!fst_hash_decode16_kzhash (hash, hash_str))
	{
		fst_hash_free (hash);
		free (hash);
		free (url0);
		return NULL;
	}

	/* parse params if that was requested */
	if (params && (*params = dataset_new (DATASET_HASH)))
	{
		char *name, *value;

		while ((value = string_sep (&param_str, "&")))
		{
			if ((name = string_sep (&value, "=")))
			{
				string_lower (name);
				dataset_insertstr (params, name, value);
			}
		}
		if ((name = string_sep (&param_str, "=")))
		{
			string_lower (name);
			dataset_insertstr (params, name, param_str);
		}
	}

	free (url0);
	return hash;
}

/*****************************************************************************/


/* http client callback */
static int download_client_callback (FSTHttpClient *client,
									 FSTHttpClientCbCode code)
{
	Source *source = (Source*) client->udata;
	assert (source);

	switch (code)
	{
	case HTCL_CB_CONNECT_FAILED:
		download_error_gift (source, TRUE, SOURCE_TIMEOUT, "Connect failed");
		break;

	case HTCL_CB_REQUESTING:
		FST_PROTO->source_status (FST_PROTO, source, SOURCE_WAITING,
								  "Requesting");
		break;

	case HTCL_CB_REQUEST_FAILED:
/*
		download_error_gift (source, TRUE, SOURCE_TIMEOUT, "Request failed");
*/
		download_error_gift (source, FALSE, SOURCE_TIMEOUT, "Request failed");
		break;
	
	case HTCL_CB_REPLIED:
	{
		FSTHttpHeader *reply = client->reply;
		char *p;

		/* check reply code */
		if (reply->code != 200 && reply->code != 206)
		{
			FST_HEAVY_DBG_3 ("got reply code %d (\"%s\") for url %s -> aborting d/l",
							 reply->code, reply->code_str, source->url);

			switch (reply->code)
			{
			case 503:
				download_error_gift (source, FALSE, SOURCE_QUEUED_REMOTE,
									 "Remotely queued");
				break;
			case 404:
				download_error_gift (source, TRUE, SOURCE_CANCELLED,
									 "File not found");
				break;
			default:
				download_error_gift (source, TRUE, SOURCE_CANCELLED,
									 "Weird http code");
			}

			return FALSE;
		}

		FST_HEAVY_DBG_3 ("got reply code %d (\"%s\") for url %s -> starting d/l",
						 reply->code, reply->code_str, source->url);

		/* make sure the start offset is correct */
		if ( (p = fst_http_header_get_field (reply, "Content-Range")))
		{
			int start, stop;
			sscanf (p, "bytes %d-%d", &start, &stop);

			/* check start offset, shorter/longer ranges are handled by giFT */
			if (start != source->chunk->start + source->chunk->transmit)
			{
				FST_WARN ("Removing source due to range mismatch");
				FST_WARN_2 ("\trequested range: %d-%d",
							source->chunk->start + source->chunk->transmit,
							source->chunk->stop - 1);
				FST_WARN_2 ("\treceived range: %d-%d", start, stop);
				FST_WARN_1 ("\tContent-Length: %s",
							fst_http_header_get_field (reply, "Content-Length"));

				download_error_gift (source, TRUE, SOURCE_CANCELLED,
									 "Range mismatch");
				return FALSE;
			}
		}

		FST_PROTO->source_status (FST_PROTO, source, SOURCE_ACTIVE, "Active");
		break;
	}

	case HTCL_CB_DATA:
		/* write data to file through giFT.
		 * this calls fst_giftcb_download_stop() when download is complete
		 *
		 * TODO: somehow use download_throttle here
		 */
		download_write_gift (source, client->data, client->data_len);
	
		break;

	case HTCL_CB_DATA_LAST:
		FST_HEAVY_DBG_3 ("HTCL_CB_DATA_LAST (%d/%d) for %s",
						 client->content_received, client->content_length,
						 source->url);

		if (client->data_len)
		{
			assert (client->content_length == client->content_received);
			/* last write
			 * this calls fst_giftcb_download_stop() when download is complete
			 */
			download_write_gift (source, client->data, client->data_len);
			break;
		}

		/* premature end of data */
		/* this makes giFT call fst_giftcb_download_stop() */
		download_error_gift (source, FALSE, SOURCE_CANCELLED,
							 "Cancelled remotely");
		return FALSE;
	}

	return TRUE; /* continue with request */
}

/*****************************************************************************/

static void download_write_gift (Source *source, unsigned char *data,
								 unsigned int len)
{
	FST_PROTO->chunk_write (FST_PROTO, source->chunk->transfer, source->chunk,
							source, data, len);
}

static void download_error_gift (Source *source, int remove_source,
								 unsigned short klass, char *error)
{
	if (remove_source)
	{
		FST_DBG_2 ("download error (\"%s\"), removing source %s",
				   error, source->url);
		FST_PROTO->source_status (FST_PROTO, source, klass, error);
		FST_PROTO->source_abort (FST_PROTO, source->chunk->transfer, source);
	}
	else
	{
		FST_PROTO->source_status (FST_PROTO, source, klass, error);
		/* tell giFT an error occured with this download */
		download_write_gift (source, NULL, 0);
	}
}

/*****************************************************************************/

#define SWAPU32(x) ((fst_uint32) ((((( \
((fst_uint8*)&(x))[0] << 8) | \
((fst_uint8*)&(x))[1]) << 8) | \
((fst_uint8*)&(x))[2]) << 8) | \
((fst_uint8*)&(x))[3])

/* returns static base64 encoded string for X-Kazaa-XferUid http header.
 * needs more work, need to figure out how last_search_hash is created.
 */
static char *download_calc_xferuid (char *uri)
{
	/* This search hash was provided by ashton.
	 * The PL changes based on system time of uploading host.
	 * I presume current time is used in creation of last_search_hash.
	 * This hash was created in the distant future and seems to give
	 * a PL of at least 1000 even though it fluctuates.
	 */
	static const unsigned char last_search_hash[32] = {
		0x6f, 0xad, 0x17, 0x55, 0x60, 0x93, 0x31, 0x0e,
		0x05, 0x69, 0x0e, 0x1f, 0xee, 0x79, 0x39, 0x60,
		0xd7, 0x47, 0xa0, 0x34, 0x20, 0x94, 0x2b, 0xf8,
		0xd7, 0xc4, 0xd8, 0xe5, 0xba, 0xf3, 0xe2, 0x97
	};

	static const unsigned int VolumeId = 0xE09C4791;


	FSTCipher *cipher;
	unsigned int buf[8]; 
	static char base64[45], *base64_ptr;
	unsigned int uri_smhash, smhash;
	unsigned int seed;

	if (uri == NULL)
		return NULL;

	if (*uri == '/')
		uri++;

	uri_smhash = fst_hash_small (0xFFFFFFFF, uri, strlen(uri));

	memcpy (buf, last_search_hash, 32);
	seed = SWAPU32 (buf[0]);

	/* run through cipher */
	cipher = fst_cipher_create ();

	if (!fst_cipher_init (cipher, seed, 0xB0))
	{
		fst_cipher_free (cipher);
		base64[0] = 0;
		return base64;
	}

	fst_cipher_crypt (cipher, (unsigned char*)(buf+1), 28); 
	fst_cipher_free (cipher);

	seed = SWAPU32 (buf[1]);

	buf[1] = 0;
	smhash = fst_hash_small (0xFFFFFFFF, (unsigned char*)(buf+1), 28);

	/* weird */
	if( (seed != smhash) ||
		(SWAPU32(buf[2]) != VolumeId) ||
		(SWAPU32(buf[6]) >= 0x3B9ACA00) ||
		(SWAPU32(buf[7]) >= 0x3B9ACA00) ||
		(SWAPU32(buf[4]) >= 0x3B9ACA00) ||
		(SWAPU32(buf[5]) >= 0x3B9ACA00))
	{
		memset(buf, 0, 32);
	}

	seed = SWAPU32 (buf[3]) - time(NULL);	/* hmm */

	buf[3] = SWAPU32 (seed);
	buf[2] = SWAPU32 (uri_smhash);

	buf[1] = 0;
	smhash = fst_hash_small (0xFFFFFFFF, (unsigned char*)(buf+1), 28);
	buf[1] = SWAPU32 (smhash);

	seed = SWAPU32 (buf[3]);
	seed ^= smhash;

	buf[0] = SWAPU32 (seed);

	/* run through cipher */
	cipher = fst_cipher_create();

	if (!fst_cipher_init (cipher, seed, 0xB0))
	{
		fst_cipher_free (cipher);
		base64[0] = 0;
		return base64;
	}

	fst_cipher_crypt (cipher, (unsigned char*)(buf+1), 28); 
	fst_cipher_free (cipher);

	/* base64 encode */
	base64_ptr = fst_utils_base64_encode ( (unsigned char*)buf, 32);
	strncpy (base64, base64_ptr, 45);
	base64[44] = '\0';
	free (base64_ptr);
	
	return base64;
}

/*****************************************************************************/

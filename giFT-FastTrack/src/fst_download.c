/*
 * $Id: fst_download.c,v 1.16 2003/09/11 17:23:47 mkern Exp $
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
static unsigned char *download_parse_url (char *url, in_addr_t *ip,
										  in_port_t *port, Dataset **params);
static char *download_parse_url_old (char *url, in_addr_t *ip, in_port_t *port);
static char *download_calc_xferuid (char *uri);

/*****************************************************************************/

/* called by gift to start downloading of a chunk */
int fst_giftcb_download_start (Protocol *p, Transfer *transfer, Chunk *chunk,
							   Source *source)
{
	FSTHttpClient *client = (FSTHttpClient*) source->udata;
	FSTHttpHeader *request;
	in_addr_t ip;
	in_port_t port;
	char *uri, *range_str;

	/* parse url */
	if (! (uri = download_parse_url_old (source->url, &ip, &port)))
	{
		/* try new url format */
		unsigned char *hash = download_parse_url (source->url, &ip, &port, NULL);
		char *hash_hex;
		
		if (!hash)
		{
			FST_WARN_1 ("malformed url %s", source->url);
			return FALSE;
		}

		hash_hex = fst_utils_hex_encode (hash, FST_HASH_LEN);
		uri = stringf_dup ("/.hash=%s", hash_hex);
		free (hash);
		free (hash_hex);
	}

	/* create http request */
	if (! (request = fst_http_header_request (HTHD_VER_11, HTHD_GET, uri)))
	{
		FST_WARN_1 ("creation of request failed for url %s", source->url);
		return FALSE;
	}

	/* if this source is used for the first time create http client */
	if (!client)
	{
		FST_HEAVY_DBG_1 ("first time use of source %s", source->url);

		client = fst_http_client_create (net_ip_str (ip), port,
										 download_client_callback);
		client->udata = (void*) source;
		source->udata = (void*) client;
	}

	/* add some http headers */
	fst_http_header_set_field (request, "UserAgent", "giFT-FastTrack");
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

/* called by gift to stop download */
void fst_giftcb_download_stop (Protocol *p, Transfer *transfer, Chunk *chunk,
							   Source *source, int complete)
{
	FSTHttpClient *client = (FSTHttpClient*) source->udata;

	/* close connection if there is outstanding data */
	if (client && client->state != HTCL_CONNECTED)
	{
		FST_HEAVY_DBG_1 ("request cancelled for url %s", source->url);
		fst_http_client_cancel (client);
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

	if (client)
	{
		FST_HEAVY_DBG_1 ("removing source %s", source->url);
		fst_http_client_free (client);
		source->udata = NULL;
	}
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
		download_error_gift (source, TRUE, SOURCE_TIMEOUT, "Request failed");
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

/* parses new format url
 * returns hash of FST_HASH_LEN size which caller frees or NULL on failure
 * params receives a dataset with additional params, caller frees, may be NULL
 */
static unsigned char *download_parse_url (char *url, in_addr_t *ip,
										  in_port_t *port, Dataset **params)
{
	char *url0, *hash_str, *param_str;
	char *ip_str, *port_str;
	unsigned char *hash;
	int hash_len;

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

	if (! (hash = fst_utils_base64_decode (hash_str, &hash_len))
		|| hash_len != FST_HASH_LEN)
	{
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
				dataset_insertstr (params, name, value);
		}
		if ((name = string_sep (&param_str, "=")))
			dataset_insertstr (params, name, param_str);
	}

	free (url0);
	return hash;
}

/* parses old format url.
 * returns uri which caller frees or NULL on failure.
 * also returns NULL in case of new url format
 */
static char *download_parse_url_old (char *url, in_addr_t *ip, in_port_t *port)
{
	char *tmp, *uri, *ip_str, *port_str;

	if (!url)
		return NULL;

	tmp = uri = strdup (url);

	string_sep (&uri, "://");       /* get rid of this useless crap */

	/* divide the string into two sides */
	if ( (port_str = string_sep (&uri, "/")))
	{
		/* pull off the left-hand operands */
		ip_str     = string_sep (&port_str, ":");

		if (ip_str && port_str)
		{
			if (ip)
				*ip = net_ip (ip_str);
			if (port)
				*port = ATOI (port_str);

			if (strncmp (uri, ".hash", 5) == 0)
			{
				uri--; *uri = '/';
				uri = strdup (uri);
				free (tmp);
				return uri;
			}
		}
	}

	free (tmp);
	return NULL;
}

/*****************************************************************************/

#define SWAPU32(x) ( ((((( \
((fst_uint8*)&(x))[0] << 8) | \
((fst_uint8*)&(x))[1]) << 8) | \
((fst_uint8*)&(x))[2]) << 8) | \
((fst_uint8*)&(x))[3])

/* returns static base64 encoded string for X-Kazaa-XferUid http header.
 * need to verify if this really works
 */
static char *download_calc_xferuid (char *uri)
{
/*
*	static const unsigned char last_search_hash[32] = {
*		0xbd, 0x48, 0xa4, 0x20, 0x85, 0x4c, 0x2d, 0x30,
*		0xee, 0x07, 0xfd, 0x6c, 0x0f, 0x0b, 0x7c, 0xf7,
*		0x7a, 0xe5, 0x86, 0x41, 0xf8, 0x29, 0x28, 0xbc,
*		0x78, 0xd4, 0xc5, 0x86, 0x6a, 0xd5, 0xde, 0xc7
*	};
*/
	static const unsigned char last_search_hash[32] = {
		0x34, 0x1c, 0x58, 0x01, 0x4d, 0x32, 0xda, 0xeb,
		0xae, 0xe7, 0x32, 0xdc, 0x60, 0xe8, 0x31, 0x76,
		0x1d, 0x47, 0xd7, 0x40, 0x0b, 0x82, 0x4e, 0x41,
		0xe7, 0xef, 0x5c, 0xd1, 0xc0, 0xa7, 0xd0, 0x79
	};

	static const unsigned int VolumeId = 0xB080A125;

	FSTCipher *cipher;
	unsigned int buf[8]; 
	static char base64[40], *base64_ptr;
	unsigned int uri_smhash, smhash;
	unsigned int seed;

	if (uri == NULL)
		return NULL;

	if (*uri == '/')
		uri++;

	uri_smhash = fst_hash_small (uri, strlen(uri), 0xFFFFFFFF);

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
	smhash = fst_hash_small ( (unsigned char*)(buf+1), 28, 0xFFFFFFFF);

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
	smhash = fst_hash_small ( (unsigned char*)(buf+1), 28, 0xFFFFFFFF);
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

	// base64 encode		
	base64_ptr = fst_utils_base64_encode ( (unsigned char*)buf, 32);
	strncpy (base64, base64_ptr, 40);
	base64[39] = '\0';
	free (base64_ptr);
	
	return base64;
}

/*****************************************************************************/

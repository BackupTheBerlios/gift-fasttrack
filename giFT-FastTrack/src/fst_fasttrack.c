/*
 * $Id: fst_fasttrack.c,v 1.47 2004/03/02 20:03:32 mkern Exp $
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

#include <signal.h>

#include "fst_fasttrack.h"

/*****************************************************************************/

Protocol *fst_proto = NULL;

/*****************************************************************************/

static void fst_plugin_connect_next ();

static void fst_plugin_discover_callback (FSTUdpDiscover *discover,
                                          FSTUdpNodeState node_state,
                                          FSTNode *node);

static int fst_plugin_session_callback (FSTSession *session,
										FSTSessionMsg msg_type,
										FSTPacket *msg_data);

/*****************************************************************************/

/*
 * Note: the connect control flow is wicked. modify with extreme care.
 */

static void fst_plugin_connect_next ()
{
	FSTNode *node;

	/* close old session if any */
	if (FST_PLUGIN->session)
	{
		/* remove old node from node cache */
		if (FST_PLUGIN->session->node)
		{
			fst_nodecache_remove (FST_PLUGIN->nodecache,
								  FST_PLUGIN->session->node->host);
		}
		/* free old session */
		fst_session_free (FST_PLUGIN->session);
		FST_PLUGIN->session = NULL;
	}

	/* connect to head node in node cache */
	while (1)
	{
		if (!(node = fst_nodecache_get_front (FST_PLUGIN->nodecache)))
		{
			/* node cache empty */
			FST_ERR ("All attempts at contacting peers have failed. Get a new nodes file.");
			return;
		}

		/* don't connect to banned ips */
		if (config_get_int (FST_PLUGIN->conf, "main/banlist_filter=0") &&
			fst_ipset_contains (FST_PLUGIN->banlist, net_ip (node->host)))
		{
			FST_DBG_2 ("not connecting to banned supernode %s:%d",
			           node->host, node->port);
			/* remove this node from cache */
			fst_nodecache_remove (FST_PLUGIN->nodecache, node->host);
			fst_node_free (node);
			continue;
		}

		/* create session and connect */
		FST_PLUGIN->session = fst_session_create (fst_plugin_session_callback);
	
		if (fst_session_connect (FST_PLUGIN->session, node))
		{
			break;
		}
		else
		{
			/* TODO: if connect fails due to net down idle for a while */

			/* free session */
			fst_session_free (FST_PLUGIN->session);
			FST_PLUGIN->session = NULL;
			/* remove this node from cache */
			fst_nodecache_remove (FST_PLUGIN->nodecache, node->host);
			fst_node_free (node);
			continue;
		}
	}

	/* We started a connection attempt with the head node from nodecache.
	 * Try to quickly find some online nodes with udp in parallel now.
	 */
	if (FST_PLUGIN->discover && FST_PLUGIN->discover->pinged_nodes == 0)
	{
		List *item = FST_PLUGIN->nodecache->list;
		int i = 0;

		while (i < FST_UDP_DISCOVER_MAX_PINGS && item && item->data)
		{
			node = (FSTNode*)item->data;

			if (!fst_udp_discover_ping_node (FST_PLUGIN->discover, node))
			{
				/* This may fail due to the network being down. While 
				 * we could handle this in a special way doing nothing
				 * works fine if being somewhat inefficient. */
			}

			item = item->next;
			i++;
		}
	}
}

/*****************************************************************************/

static void fst_plugin_discover_callback (FSTUdpDiscover *discover,
                                          FSTUdpNodeState node_state,
                                          FSTNode *node)
{
	switch (node_state)
	{
	case UdpNodeStateDown:
		/* remove this node from node cache _if_ we know that udp works.
		 * otherwise just move the node to the back of the cache.
		 */
		if (FST_PLUGIN->discover->udp_working)
		{
			FST_HEAVY_DBG_2 ("UdpNodeStateDown: %s:%d, UDP works",
			                 node->host, node->port);
			fst_nodecache_remove (FST_PLUGIN->nodecache, node->host);
		}
		else
		{
			FST_HEAVY_DBG_2 ("UdpNodeStateDown: %s:%d, UDP not verified",
			                 node->host, node->port);
			/* remove node from current position in cache... */
			fst_nodecache_remove (FST_PLUGIN->nodecache, node->host);
			/* ... and add it to the back */
			fst_nodecache_add (FST_PLUGIN->nodecache, node->klass, node->host,
				               node->port, 100, node->last_seen);
		}
		break;
	case UdpNodeStateUp:
		FST_HEAVY_DBG_2 ("UdpNodeStateUp: %s:%d", node->host, node->port);
		/* remove node from current position in cache... */
		fst_nodecache_remove (FST_PLUGIN->nodecache, node->host);
		/* ... and add it to the back */
		fst_nodecache_add (FST_PLUGIN->nodecache, node->klass, node->host,
		                   node->port, 100, node->last_seen);
		break;
	case UdpNodeStateFree:
		FST_HEAVY_DBG_2 ("UdpNodeStateFree: %s:%d", node->host, node->port);
		/* remove node from current position in cache... */
		fst_nodecache_remove (FST_PLUGIN->nodecache, node->host);
		/* ... and add it to the front */
		fst_nodecache_add (FST_PLUGIN->nodecache, node->klass, node->host,
		                   node->port, 0, node->last_seen);

/*
 * Enabling this has proven to be a bad idea since it terminates connections
 * before they are fully established.		
 */
#if 0
		/* if we don't have a established session try again with this node */
		if (!FST_PLUGIN->session || FST_PLUGIN->session->state == SessConnecting)
		{
			FST_HEAVY_DBG ("no established session, using this node...");
			/* this calls us back with SessMsgDisconnected */
			fst_session_disconnect (FST_PLUGIN->session);
		}
#endif

		break;
	}
}

static int fst_plugin_session_callback (FSTSession *session,
										FSTSessionMsg msg_type,
										FSTPacket *msg_data)
{
	switch (msg_type)
	{
	/* session management messages */
	case SessMsgConnected:
	{
		/* determine local ip */
		FST_PLUGIN->local_ip = net_local_ip (session->tcpcon->fd, NULL);
		FST_DBG_1 ("connected, local ip: %s", net_ip_str (FST_PLUGIN->local_ip));
		break;
	}

	case SessMsgEstablished:
	{
		FST_DBG_3 ("supernode connection established to %s:%d, load: %d%%",
				   session->node->host, session->node->port, session->node->load);

		/* resend queries for all running searches */
		fst_searchlist_send_queries (FST_PLUGIN->searches, session, TRUE);

		break;
	}

	case SessMsgDisconnected:
	{
		/* zero stats */
		FST_PLUGIN->stats->users = 0;
		FST_PLUGIN->stats->files = 0;
		FST_PLUGIN->stats->size = 0;

		/* reset external ip because we wait with sharing until we get it again */
		FST_PLUGIN->external_ip = 0;

		fst_plugin_connect_next ();
		return FALSE;
	}

	/* FastTrack messages */
	case SessMsgNodeList:
	{
		int i;
		time_t now = time (NULL); 

		for (i=0; fst_packet_remaining (msg_data) >= 8; i++)
		{
			unsigned long ip		= fst_packet_get_uint32 (msg_data);			
			unsigned short port		= ntohs (fst_packet_get_uint16 (msg_data));	
			unsigned int last_seen	= fst_packet_get_uint8 (msg_data);			
			unsigned int load		= fst_packet_get_uint8 (msg_data);		
/*
			FST_DBG_4 ("node: %s:%d   load: %d%% last_seen: %d mins ago",
					   net_ip_str(ip), port, load, last_seen);
*/
			fst_nodecache_add (FST_PLUGIN->nodecache, NodeKlassSuper,
							   net_ip_str (ip), port, load, now - last_seen * 60);
		}

		/* sort the cache again */
		fst_nodecache_sort (FST_PLUGIN->nodecache);

		FST_DBG_1 ("added %d received supernode IPs to nodes list", i);

		/* if we got this from an index node disconnect now and use a supernode */
		if (session->node->klass == NodeKlassIndex)
		{
			FST_DBG ("disconnecting from index node");
			/* this calls us back with SessMsgDisconnected */
			fst_session_disconnect (session);
			return FALSE;
		}
		break;
	}

	case SessMsgNetworkStats:
	{
		unsigned int mantissa, exponent;

		if (fst_packet_remaining (msg_data) < 12)
			break;

		FST_PLUGIN->stats->users = ntohl (fst_packet_get_uint32 (msg_data));
		FST_PLUGIN->stats->files = ntohl (fst_packet_get_uint32 (msg_data));

		mantissa = ntohs(fst_packet_get_uint16 (msg_data));	/* mantissa of size */
		exponent = ntohs(fst_packet_get_uint16 (msg_data));	/* exponent of size */

    	if (exponent >= 30)
			FST_PLUGIN->stats->size = mantissa << (exponent - 30);
    	else
			FST_PLUGIN->stats->size = mantissa >> (30 - exponent);

		/* what follows in the packet is the number of files and their size
		 * per media type (6 times).
		 * Then optionally the different network names and the number of
		 * their users.
		 * we do not currently care for those
		 */

		FST_DBG_3 ("received network stats: %d users, %d files, %d GB",
				   FST_PLUGIN->stats->users,
				   FST_PLUGIN->stats->files,
				   FST_PLUGIN->stats->size);
		break;
	}

	case SessMsgNetworkName:
	{
		FSTPacket *packet;
		char *net_name = STRDUP_N (msg_data->data, fst_packet_size(msg_data));

		FST_DBG_2 ("received network name: \"%s\", sending ours: \"%s\"",
				   net_name ,FST_NETWORK_NAME);
		free (net_name);

		if (! (packet = fst_packet_create ()))
			break; /* not overly important, just don't send it */

		fst_packet_put_ustr (packet, FST_NETWORK_NAME, strlen (FST_NETWORK_NAME));

		if (!fst_session_send_message (session, SessMsgNetworkName, packet))
		{
			fst_packet_free (packet);
			fst_session_disconnect (session);
			return FALSE;
		}

		fst_packet_free (packet);
		break;
	}

	case SessMsgExternalIp:
	{
		FST_PLUGIN->external_ip = fst_packet_get_uint32 (msg_data);
		FST_DBG_1 ("received external ip: %s",
				   net_ip_str (FST_PLUGIN->external_ip));

		/* upload our shares to supernode.
		 * we do this here because we have to make sure we are accessible
		 * form the outside since we don't push yet.
		 */
		if (fst_share_do_share ())
		{
			FST_DBG ("registering shares with new supernode");
			if (!fst_share_register_all ())
				FST_DBG ("registering shares with new supernode failed");
		}

		break;
	}

	case SessMsgQueryReply:
	case SessMsgQueryEnd:
	{
		fst_searchlist_process_reply (FST_PLUGIN->searches, msg_type, msg_data);
		break;
	}

	default:
/*
		FST_DBG_2 ("unhandled message: type = 0x%02x, length = %d",
				   msg_type, fst_packet_size(msg_data));
		printf("\nunhandled message: type = 0x%02x, length = %d",
			   msg_type, fst_packet_size(msg_data));
		print_bin_data(msg_data->data, fst_packet_remaining(msg_data));
*/
		break;
	}

	return TRUE;
}

/*****************************************************************************/

int copy_default_file (const char *filename)
{
	char *local_path, *default_path, *target_dir;

	target_dir = stringf_dup ("%s/FastTrack", platform_local_dir());
	local_path = stringf_dup ("%s/FastTrack/%s", platform_local_dir(), filename);
	default_path = stringf_dup ("%s/FastTrack/%s", platform_data_dir(), filename);

	if (!file_exists (local_path))
	{
		FST_WARN_2 ("Local file \"%s\" does not exist, copying default from \"%s\"",
					local_path, default_path);

		/* make sure the target directory exists */
		if (!file_mkdir (target_dir, 0777))
		{
			FST_ERR_1 ("Unable to create directory \"%s\"", target_dir);

			free (target_dir);
			free (local_path);
			free (default_path);
			return FALSE;
		}


		if (!file_cp (default_path, local_path))
		{		
			FST_ERR_1 ("Unable to copy default file \"%s\"", default_path);

			free (target_dir);
			free (local_path);
			free (default_path);
			return FALSE;
		}
	}

	free (target_dir);
	free (local_path);
	free (default_path);
	return TRUE;
}

/*****************************************************************************/

/* allocate and init plugin */
static int fst_giftcb_start (Protocol *p)
{
	FSTPlugin *plugin;
	int i;
	char *filepath;
	in_port_t server_port;

	FST_DBG ("starting up");

	if (! (plugin = malloc (sizeof (FSTPlugin))))
		return FALSE;

	/* init config and copy to local config if missing */
	copy_default_file ("FastTrack.conf");
	
	if (! (plugin->conf = gift_config_new ("FastTrack")))
	{
		free (plugin);
		FST_ERR ("Unable to open fasttrack configuration, exiting plugin.");
		return FALSE;
	}

	/* set protocol pointer */
	p->udata = (void*)plugin;

	/* cache user name */
	FST_PLUGIN->username = strdup (config_get_str (FST_PLUGIN->conf,
	                                               "main/alias=giFTed"));

	/* init node cache */
	FST_PLUGIN->nodecache = fst_nodecache_create ();

	/* load nodes file, copy default if necessary */
	copy_default_file ("nodes");

	filepath = gift_conf_path ("FastTrack/nodes");
	i = fst_nodecache_load (plugin->nodecache, filepath);

	if (i < 0)
		FST_WARN_1 ("Couldn't find nodes file \"%s\". Fix that!", filepath);
	else
		FST_DBG_2 ("Loaded %d supernode addresses from nodes file \"%s\"",
	               i, filepath);

	/* create list of banned ips */
	FST_PLUGIN->banlist = fst_ipset_create ();

	/* load ban list, copy default if necessary */
	copy_default_file ("banlist");

	filepath = gift_conf_path ("FastTrack/banlist");
	i = fst_ipset_load (FST_PLUGIN->banlist, filepath);

	if(i < 0)
		FST_WARN_1 ("Couldn't find banlist \"%s\"", filepath);
	else
		FST_DBG_2 ("Loaded %d banned ip ranges from \"%s\"", i, filepath);

	/* attempt to start http server */
	FST_PLUGIN->server = NULL;
	server_port = config_get_int (FST_PLUGIN->conf, "main/port=0");

	if (server_port)
	{
		FST_PLUGIN->server = fst_http_server_create (server_port,
		                                             fst_upload_process_request,
		                                             fst_push_process_reply,
		                                             NULL);

		if (!FST_PLUGIN->server)
		{
			FST_WARN_1 ("Couldn't bind to port %d. Http server not started.",
			            server_port);
		}
		else
		{
			FST_DBG_1 ("Http server listening on port %d", server_port);
		}
	}
	else
	{
		FST_DBG ("Port set to zero. Http server not started.");
	}

	/* set session to NULL */
	FST_PLUGIN->session = NULL;

	/* create discover */
	FST_PLUGIN->discover = fst_udp_discover_create (fst_plugin_discover_callback);

	if (!FST_PLUGIN->discover)
	{
		FST_WARN ("Creation of udp discovery failed");
	}
	
	/* init searches */
	FST_PLUGIN->searches = fst_searchlist_create();

	/* init stats */
	FST_PLUGIN->stats = fst_stats_create ();

	/* init push list */
	FST_PLUGIN->pushlist = fst_pushlist_create ();

	/* get forwarded port from config file */
	FST_PLUGIN->forwarding = config_get_int (FST_PLUGIN->conf,
	                                         "main/forwarding=0");
	FST_PLUGIN->local_ip = 0;
	FST_PLUGIN->external_ip = 0;
	
	/* make shares visible until giFT tells us otherwise */
	FST_PLUGIN->hide_shares = FALSE;

	/* cache allow_sharing key */
	FST_PLUGIN->allow_sharing = config_get_int (FST_PLUGIN->conf,
	                                            "main/allow_sharing=0");

#if 0
	/*
	 * add some static nodes for faster startup
	 * note: apparently fm2.imesh.com sends udp replies from different ips.
	 * for security reasons we drop those.
	 */
	FST_DBG ("adding fm2.imesh.com:1214 as index node");
	fst_nodecache_add (FST_PLUGIN->nodecache, NodeKlassIndex,
					   "fm2.imesh.com", 1214, 0, time (NULL));
#endif

	/* start first connection */
	fst_plugin_connect_next ();

	return TRUE;
}

/* destroy plugin */
static void fst_giftcb_destroy (Protocol *p)
{
	char *nodesfile;
	int i;

	FST_DBG ("shutting down");

	if (!FST_PLUGIN)
		return;

	/* free push list */
	fst_pushlist_free (FST_PLUGIN->pushlist);

	/* shutdown http server */
	fst_http_server_free (FST_PLUGIN->server);

	/* free udp discovery */
	fst_udp_discover_free (FST_PLUGIN->discover);

	/* put currently used supernode at the front of the node cache */
	if (FST_PLUGIN->session && FST_PLUGIN->session->state == SessEstablished)
	{
		fst_nodecache_add (FST_PLUGIN->nodecache, NodeKlassSuper,
		                   FST_PLUGIN->session->node->host,
		                   FST_PLUGIN->session->node->port,
		                   0, time (NULL));

		FST_DBG_2 ("added current supernode %s:%d back into node cache",
		           FST_PLUGIN->session->node->host,
		           FST_PLUGIN->session->node->port);
	}

	/* free session */
	fst_session_free (FST_PLUGIN->session);

	/* free stats */
	fst_stats_free (FST_PLUGIN->stats);

	/* free searches */
	fst_searchlist_free (FST_PLUGIN->searches);

	/* free list of banned ips */
	fst_ipset_free (FST_PLUGIN->banlist);

	/* save and free nodes */
	nodesfile = gift_conf_path ("FastTrack/nodes");
	i = fst_nodecache_save (FST_PLUGIN->nodecache, nodesfile);
	if (i < 0)
		FST_WARN_1 ("couldn't save nodes file \"%s\"", nodesfile);
	else
		FST_DBG_2 ("saved %d supernode addresses to nodes file \"%s\"",
				   i, nodesfile);
	fst_nodecache_free (FST_PLUGIN->nodecache);

	/* free cached user name */
	free (FST_PLUGIN->username);

	/* free config */
	config_free (FST_PLUGIN->conf);

	free (FST_PLUGIN);
}

/*****************************************************************************/

static int fst_giftcb_source_cmp (Protocol *p, Source *a, Source *b)
{
	return strcmp (a->url, b->url);
}

static int fst_giftcb_user_cmp (Protocol *p, const char *a, const char *b)
{
	return strcmp (a, b);
}

/* TODO: move this to something like fst_transfer.c */

static int fst_giftcb_chunk_suspend (Protocol *p, Transfer *transfer,
                                     Chunk *chunk, Source *source)
{
	if (transfer_direction (transfer) == TRANSFER_UPLOAD)
	{
		FSTUpload *upload = (FSTUpload*) chunk->udata;
		assert (upload);

		input_suspend_all (upload->tcpcon->fd);
	}
	else
	{
		FSTHttpClient *client = (FSTHttpClient*) source->udata;
		assert (client);

		input_suspend_all (client->tcpcon->fd);
	}
	
	return TRUE;
}

static int fst_giftcb_chunk_resume (Protocol *p, Transfer *transfer,
                                    Chunk *chunk, Source *source)
{
	if (transfer_direction (transfer) == TRANSFER_UPLOAD)
	{
		FSTUpload *upload = (FSTUpload*) chunk->udata;
		assert (upload);

		input_resume_all (upload->tcpcon->fd);
	}
	else
	{
		FSTHttpClient *client = (FSTHttpClient*) source->udata;
		assert (client);

		input_resume_all (client->tcpcon->fd);
	}

	return TRUE;
}

/*****************************************************************************/

static void fst_plugin_setup_functbl (Protocol *p)
{
	/*
	 * communicate special properties of this protocol which will modify
	 * giFT's behaviour
 	 * NOTE: most of these dont do anything yet
	 */

	p->support (p, "range-get", TRUE);
	p->support (p, "hash-unique", TRUE);

	/*
	 * Finally, assign the support communication structure.
	 */

	/* fst_hash.c */
	p->hash_handler (p, (const char*)FST_HASH_NAME, HASH_PRIMARY,
					 (HashFn)fst_giftcb_FTH, (HashDspFn)fst_giftcb_FTH_human);

	/* fst_fasttrack.c */
	p->start          = fst_giftcb_start;
	p->destroy        = fst_giftcb_destroy;
	p->source_cmp     = fst_giftcb_source_cmp;
	p->user_cmp       = fst_giftcb_user_cmp;
	p->chunk_suspend  = fst_giftcb_chunk_suspend;
	p->chunk_resume   = fst_giftcb_chunk_resume;

	/* fst_search.c */
	p->search         = fst_giftcb_search;
	p->browse         = fst_giftcb_browse;
	p->locate         = fst_giftcb_locate;
	p->search_cancel  = fst_giftcb_search_cancel;

	/* fst_download.c */
	p->download_start = fst_giftcb_download_start;
	p->download_stop  = fst_giftcb_download_stop;
	p->source_add     = fst_giftcb_source_add;
	p->source_remove  = fst_giftcb_source_remove;

	/* fst_upload.c */
	p->upload_stop    = fst_giftcb_upload_stop;

	/* fst_share.c */
	p->share_new      = fst_giftcb_share_new;
	p->share_free     = fst_giftcb_share_free;
	p->share_add      = fst_giftcb_share_add;
	p->share_remove   = fst_giftcb_share_remove;
	p->share_sync     = fst_giftcb_share_sync;
	p->share_hide     = fst_giftcb_share_hide;
	p->share_show     = fst_giftcb_share_show;

	/* fst_stats.c */
	p->stats          = fst_giftcb_stats;
}

int FastTrack_init (Protocol *p)
{
	/* make sure we're loaded with the correct plugin interface version */
	if (protocol_compat (p, LIBGIFTPROTO_MKVERSION (0, 11, 5)) != 0)
		return FALSE;
	
	/* tell giFT about our version
	 * VERSION is defined in config.h. e.g. "0.8.2"
	 */ 
	p->version_str = strdup (VERSION);
	
	/* put protocol in global variable so we always have access to it */
	fst_proto = p;

	/* setup giFT callbacks */
	fst_plugin_setup_functbl (p);

	return TRUE;
}

/*****************************************************************************/

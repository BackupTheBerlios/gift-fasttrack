/*
 * $Id: fst_udp_discover.c,v 1.2 2004/01/01 22:45:18 mkern Exp $
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
#include "fst_udp_discover.h"

/*****************************************************************************/

static void udp_discover_receive (int fd, input_id input,
                                  FSTUdpDiscover *discover);
static int udp_discover_timeout (FSTUdpDiscover *discover);
static int udp_discover_send_ping (FSTUdpDiscover *discover, FSTNode *node);

static int udpsock_bind (in_port_t port, int blocking);

/*****************************************************************************/

FSTUdpNode* fst_udp_node_create (in_addr_t ip, in_addr_t port)
{
	FSTUdpNode *udp_node = malloc (sizeof (FSTUdpNode));

	if (!udp_node)
		return NULL;

	udp_node->ip = ip;
	udp_node->port = port;
	udp_node->state = UdpNodeStateNew;
	udp_node->last_seen = 0;
	udp_node->min_enc_type = 0x00;
	udp_node->network = NULL;

	return udp_node;
}

void fst_udp_node_free (FSTUdpNode* udp_node)
{
	if (!udp_node)
		return;

	free (udp_node->network);
	free (udp_node);
}

/*****************************************************************************/

FSTUdpDiscover *fst_udp_discover_create (FSTUdpDiscoverCallback callback,
                                         FSTNodeCache *cache)
{
	FSTUdpDiscover *discover = malloc (sizeof (FSTUdpDiscover));
	in_port_t port;

	if (!discover)
		return NULL;

	discover->nodes = NULL;
	discover->pinged_nodes = 0;
	discover->cache = cache;
	discover->callback = callback;
	discover->timer = 0;

	if ((port = config_get_int (FST_PLUGIN->conf, "main/port=0")) == 0)
		port = 1214;

	/* bind socket */
	if ((discover->fd = udpsock_bind (port, FALSE)) < 0)
	{
		free (discover);
		FST_DBG ("binding UDP sock for discovery failed");
		return NULL;
	}

	/* sort the cache so we start with the best nodes */
	fst_nodecache_sort (discover->cache);

	/* send initial pings */
	while (discover->pinged_nodes < FST_UDP_DISCOVER_MAX_PINGS)
	{
		FSTNode *node = fst_nodecache_get_front (discover->cache);
		
		if(!node)
			break;

		/* remove this node from cache */
		fst_nodecache_remove (discover->cache, node->host);

		udp_discover_send_ping (discover, node);

		fst_node_free (node);
	}

	if (discover->pinged_nodes == 0)
	{
		fst_udp_discover_free (discover, FALSE);
		FST_DBG ("no nodes to ping");
		return NULL;
	}

	/* create timer for timeout */
	discover->timer = timer_add (FST_UDP_DISCOVER_TIMEOUT,
	                             (TimerCallback)udp_discover_timeout,
	                             discover);

	if (!discover->timer)
	{
		fst_udp_discover_free (discover, TRUE);
		FST_DBG ("timer init_failed");
		return NULL;
	}

	/* wait for responses */
	input_add (discover->fd, (void*) discover, INPUT_READ,
			   (InputCallback) udp_discover_receive, 0);

	return discover;
}


/* free udp node and optionally save it back to cache */
static int udp_discover_free_node (FSTUdpNode *udp_node,
                                   FSTNodeCache *cache)
{
	if (cache)
	{
		unsigned int load = 0;
		fst_nodecache_add (cache, NodeKlassSuper, net_ip_str (udp_node->ip),
		                   udp_node->port, load, udp_node->last_seen);
		FST_HEAVY_DBG_2 ("added node %s:%d back into cache",
		                 net_ip_str (udp_node->ip), udp_node->port);
	}
	
	fst_udp_node_free (udp_node);
	return TRUE;
}

void fst_udp_discover_free (FSTUdpDiscover *discover, int save_nodes)
{
	if (!discover)
		return;

	/* remove all inputs */
	timer_remove (discover->timer);
	input_remove_all (discover->fd);
	net_close (discover->fd);
	discover->fd = -1;

	if (save_nodes)
	{
		list_foreach_remove (discover->nodes,
		                     (ListForeachFunc)udp_discover_free_node,
	                         discover->cache);
		/* sort cache again */
		fst_nodecache_sort (discover->cache);
	}
	else
	{
		list_foreach_remove (discover->nodes,
		                     (ListForeachFunc)udp_discover_free_node,
	                         NULL);
	}

	free (discover);
}


FSTNode *fst_udp_discover_get_node (FSTUdpDiscover *discover)
{
	List *item;
	FSTNode *node;
	FSTUdpNode *udp_node;

	if (!discover)
		return NULL;

	/* find the first responsive node and return it */
	for (item=discover->nodes; item; item=item->next)
	{
		udp_node = (FSTUdpNode*)item->data;

		if (udp_node->state == UdpNodeStateUp)
			break;
	}

	if (!item)
		return NULL;

	discover->nodes = list_remove_link(discover->nodes, item);	
	
	node = fst_node_create (NodeKlassSuper, net_ip_str (udp_node->ip),
	                        udp_node->port, 0, udp_node->last_seen);

	fst_udp_node_free (udp_node);
	
	if (!node)
		return NULL;

	return node;
}

/*****************************************************************************/

static void udp_discover_receive (int fd, input_id input,
                                  FSTUdpDiscover *discover)
{
	FSTPacket *packet;
	FSTUdpNode *udp_node = NULL;
	List *udp_node_link = NULL;
	FSTNode *node;
	unsigned char buf[256]; /* more than enough for the packet we want */
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	int len, type;
	
	if (net_sock_error (fd))
	{
		/* this should never happen */
		FST_ERR ("udp_discover_receive called with invalid fd");
		/* TODO: do something more clever than aborting */
		assert(net_sock_error (fd) == FALSE);
		return;
	}

	if ((len = recvfrom (fd, buf, 256, 0, (struct sockaddr*)&addr, &addr_len)) < 0)
	{
		FST_ERR ("udp_discover_receive: recvfrom failed");
		return;
	}

	/* find this udp node in list */
	for (udp_node_link=discover->nodes; udp_node_link; udp_node_link=udp_node_link->next)
	{
		if (((FSTUdpNode*)udp_node_link->data)->ip == addr.sin_addr.s_addr)
		{
			udp_node = udp_node_link->data;
			break;
		}
	}
	
	if (!udp_node)
	{
		FST_DBG_2 ("recevied udp reply from node %s:%d which is not in list",
		           net_ip_str (addr.sin_addr.s_addr), ntohs (addr.sin_port));
		return;
	}

	/* create packet */
	if (!(packet = fst_packet_create ()))
		return;

	fst_packet_put_ustr (packet, buf, len);
	fst_packet_rewind (packet);

	/* parse packet */
	type = fst_packet_get_uint8 (packet);

	/* update udp_node with the new data */
	udp_node->min_enc_type = ntohl (fst_packet_get_uint32 (packet));
	fst_packet_get_uint8 (packet); /* unknown (0x00) */
	fst_packet_get_uint8 (packet); /* unknown */
	fst_packet_get_uint32 (packet); /* unknown */

	if((len = fst_packet_strlen (packet, 0x00)) < 0)
	{
		FST_DBG_2 ("received corrupted udp reply from %s:%d",
		            net_ip_str (udp_node->ip), udp_node->port);
		/* remove udp_node from list */
		discover->nodes = list_remove_link(discover->nodes, udp_node_link);
		fst_udp_node_free (udp_node);
		fst_packet_free (packet);
		return;
	}

	udp_node->network = fst_packet_get_ustr (packet, len+1);
	fst_packet_free (packet);

	udp_node->last_seen = time (NULL);
	udp_node->state = UdpNodeStateUp;

	/* notify plugin using FSTNode */
	node = fst_node_create (NodeKlassSuper, net_ip_str (udp_node->ip),
	                        udp_node->port, 0, udp_node->last_seen);

	if (!node)
	{
		discover->nodes = list_remove_link(discover->nodes, udp_node_link);
		fst_udp_node_free (udp_node);
		return;
	}

	discover->pinged_nodes--;

	if (type == UdpMsgPong)
	{
		FST_DBG_4 ("received udp reply 0x%02x (pong) from %s:%d, pinged nodes: %d",
		           type, net_ip_str (udp_node->ip), udp_node->port,
		           discover->pinged_nodes);
		discover->callback (discover, node);
	}
	else
	{
		FST_DBG_4 ("received udp reply 0x%02x from %s:%d, pinged nodes: %d",
		           type, net_ip_str (udp_node->ip), udp_node->port,
		           discover->pinged_nodes);
		/* remove udp_node from list */
		discover->nodes = list_remove_link(discover->nodes, udp_node_link);
		fst_udp_node_free (udp_node);
	}

	fst_node_free (node);

	/* send next ping(s) */
	while (discover->pinged_nodes < FST_UDP_DISCOVER_MAX_PINGS)
	{
		if(!(node = fst_nodecache_get_front (discover->cache)))
			break;

		/* remove this node from cache */
		fst_nodecache_remove (discover->cache, node->host);
		udp_discover_send_ping (discover, node);
		fst_node_free (node);
	}

	/* wait for next packet */
	return;
}

static int udp_discover_timeout (FSTUdpDiscover *discover)
{
	List *item;
	unsigned int now = time (NULL);

	/* check all pending udp nodes for timeout */
	for (item=discover->nodes; item; item=item->next)
	{
		FSTUdpNode *udp_node = (FSTUdpNode*)item->data;

		if (udp_node->state != UdpNodeStatePinged)
			continue;

		if (udp_node->last_seen + (FST_UDP_DISCOVER_TIMEOUT/SECONDS) > now)
			continue;

		/* the node timed out, remove it */
		discover->nodes = list_remove_link(discover->nodes, item);	
		discover->pinged_nodes--;

		FST_HEAVY_DBG_3 ("removed timed out udp node %s:%d, pinged nodes: %d",
		                 net_ip_str (udp_node->ip), udp_node->port,
						 discover->pinged_nodes);

		fst_udp_node_free (udp_node);

		/* restart iteration at beginning of list */
		item = discover->nodes;
	}
	
	/* send next ping(s) */
	while (discover->pinged_nodes < FST_UDP_DISCOVER_MAX_PINGS)
	{
		FSTNode *node = fst_nodecache_get_front (discover->cache);

		if(!node)
			break;

		/* remove this node from cache */
		fst_nodecache_remove (discover->cache, node->host);
		udp_discover_send_ping (discover, node);
		fst_node_free (node);
	}
	
	/* raise us again after FST_UDP_DISCOVER_TIMEOUT */
	return TRUE;
}

/*****************************************************************************/

static int udp_discover_send_ping (FSTUdpDiscover *discover, FSTNode *node)
{
	struct hostent *he;
	FSTUdpNode *udp_node;
	in_addr_t ip;
	FSTPacket *packet;
	struct sockaddr_in addr;

	if (!discover || !node)
		return FALSE;

	/* TODO: make this non-blocking */
	if (! (he = gethostbyname (node->host)))
	{
		FST_WARN_1 ("udp_discover_send_ping: gethostbyname failed for host %s",
		            node->host);
		return FALSE;
	}

	ip = *((in_addr_t*)he->h_addr_list[0]);

	FST_HEAVY_DBG_3 ("sending udp ping to %s(%s):%d", node->host,
	                 net_ip_str(ip), node->port);

	/* create udp_node */
	if (!(udp_node = fst_udp_node_create (ip, node->port)))
		return FALSE;

	/* create packet */
	if (!(packet = fst_packet_create ()))
	{
		fst_udp_node_free (udp_node);
		return FALSE;
	}

	fst_packet_put_uint8 (packet, UdpMsgPing);
	fst_packet_put_uint32 (packet, htonl (0x29)); /* minimum enc_type */
	fst_packet_put_uint8 (packet, 0x80); /* unknown */
	fst_packet_put_ustr (packet, FST_NETWORK_NAME, strlen (FST_NETWORK_NAME) + 1);

	/* send packet */

	memset (&addr, 0, sizeof (addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = udp_node->ip;
	addr.sin_port        = htons (udp_node->port);

	if(sendto (discover->fd, packet->data, packet->used, 0, (struct sockaddr*)&addr, sizeof (addr)) != packet->used)
	{
		FST_DBG_2 ("sendto failed for %s:%d",
		           net_ip_str (udp_node->ip), udp_node->port);
		fst_udp_node_free (udp_node);
		fst_packet_free (packet);
		return FALSE;
	}

	fst_packet_free (packet);
	
	udp_node->state = UdpNodeStatePinged;
	udp_node->last_seen = time (NULL);

	/* add node to list */
	discover->nodes = list_append (discover->nodes, udp_node);
	discover->pinged_nodes++;

	FST_DBG_3 ("sent udp ping to %s:%d, pinged nodes: %d",
	           net_ip_str (udp_node->ip), udp_node->port,
	           discover->pinged_nodes);

	return TRUE;
}

/*****************************************************************************/

static int udpsock_bind (in_port_t port, int blocking)
{
	int fd;
	struct sockaddr_in server;
	int opt;

	assert (port > 0);

	if ((fd = socket (AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
		return fd;

	memset (&server, 0, sizeof (server));
	server.sin_family      = AF_INET;
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	server.sin_port        = htons (port);

	/* was opt = sizeof (server)...is this change correct? */
	opt = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
	net_set_blocking (fd, blocking);

	if (bind (fd, (struct sockaddr*)&server, sizeof (server)) < 0)
	{
		net_close (fd);
		return -1;
	}

	listen (fd, 5);

	return fd;
}

/*****************************************************************************/


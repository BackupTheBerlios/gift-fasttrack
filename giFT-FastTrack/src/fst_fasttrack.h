/*
 * $Id: fst_fasttrack.h,v 1.35 2004/02/29 22:15:26 mkern Exp $
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

#ifndef __FASTTRACK_H
#define __FASTTRACK_H

/*****************************************************************************/

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef GIFT_PLUGIN
# error "GIFT_PLUGIN not defined. Your build environment seems broken."
#endif

#define FILE_LINE_FUNC __FILE__,__LINE__,__PRETTY_FUNCTION__

/* The default shall be debugging on, unless it is a stable release */
#ifdef DEBUG
#define FST_DBG(fmt)			FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt)
#define FST_DBG_1(fmt,a)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a)
#define FST_DBG_2(fmt,a,b)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b)
#define FST_DBG_3(fmt,a,b,c)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c)
#define FST_DBG_4(fmt,a,b,c,d)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d)
#define FST_DBG_5(fmt,a,b,c,d,e)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d,e)
#else
#define FST_DBG(fmt)
#define FST_DBG_1(fmt,a)
#define FST_DBG_2(fmt,a,b)
#define FST_DBG_3(fmt,a,b,c)
#define FST_DBG_4(fmt,a,b,c,d)
#define FST_DBG_5(fmt,a,b,c,d,e)
#endif /* DEBUG */


#ifdef HEAVY_DEBUG
# define FST_HEAVY_DBG(fmt)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt)
# define FST_HEAVY_DBG_1(fmt,a)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a)
# define FST_HEAVY_DBG_2(fmt,a,b)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b)
# define FST_HEAVY_DBG_3(fmt,a,b,c)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c)
# define FST_HEAVY_DBG_4(fmt,a,b,c,d)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d)
# define FST_HEAVY_DBG_5(fmt,a,b,c,d,e)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d,e)
#else
# define FST_HEAVY_DBG(fmt)
# define FST_HEAVY_DBG_1(fmt,a)
# define FST_HEAVY_DBG_2(fmt,a,b)
# define FST_HEAVY_DBG_3(fmt,a,b,c)
# define FST_HEAVY_DBG_4(fmt,a,b,c,d)
# define FST_HEAVY_DBG_5(fmt,a,b,c,d,e)
#endif /* HEAVY_DEBUG */


#define FST_WARN(fmt)			FST_PROTO->warn(FST_PROTO,fmt)
#define FST_WARN_1(fmt,a)		FST_PROTO->warn(FST_PROTO,fmt,a)
#define FST_WARN_2(fmt,a,b)		FST_PROTO->warn(FST_PROTO,fmt,a,b)
#define FST_WARN_3(fmt,a,b,c)	FST_PROTO->warn(FST_PROTO,fmt,a,b,c)
#define FST_WARN_4(fmt,a,b,c,d)	FST_PROTO->warn(FST_PROTO,fmt,a,b,c,d)

#define FST_ERR(fmt)			FST_PROTO->err(FST_PROTO,fmt)
#define FST_ERR_1(fmt,a)		FST_PROTO->err(FST_PROTO,fmt,a)
#define FST_ERR_2(fmt,a,b)		FST_PROTO->err(FST_PROTO,fmt,a,b)
#define FST_ERR_3(fmt,a,b,c)	FST_PROTO->err(FST_PROTO,fmt,a,b,c)
#define FST_ERR_4(fmt,a,b,c,d)	FST_PROTO->err(FST_PROTO,fmt,a,b,c,d)

#include <libgift/libgift.h>
#include <libgift/proto/protocol.h>
#include <libgift/proto/share.h>
#include <libgift/file.h>
#include <libgift/mime.h>

#if TIME_WITH_SYS_TIME
# include <time.h>
# include <sys/time.h>
#else /* !TIME_WITH_SYS_TIME */
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif /* TIME_WITH_SYS_TIME */

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

/* Just a hack 'til we fix it properly */
#ifndef _MSC_VER
typedef int8_t   fst_int8;
typedef uint8_t  fst_uint8;
typedef int16_t  fst_int16;
typedef uint16_t fst_uint16;
typedef int32_t  fst_int32;
typedef uint32_t fst_uint32;

#else
#define fst_int8 int8_t
#define fst_uint8 uint8_t
#define fst_int16 int16_t
#define fst_uint16 uint16_t
#define fst_int32 int32_t
#define fst_uint32 uint32_t
#endif /* _MSC_VER */

/*****************************************************************************/

#include <ctype.h>
#include <string.h>

#include "fst_node.h"
#include "fst_packet.h"
#include "fst_session.h"
#include "fst_hash.h"
#include "fst_search.h"
#include "fst_download.h"
#include "fst_stats.h"
#include "fst_utils.h"
#include "fst_meta.h"
#include "fst_ipset.h"
#include "fst_http_server.h"
#include "fst_push.h"
#include "fst_upload.h"
#include "fst_share.h"
#include "fst_udp_discover.h"

/*****************************************************************************/

#define FST_PLUGIN ( (FSTPlugin*)fst_proto->udata)
#define FST_PROTO (fst_proto)

/* network name we send to supernodes */
#define FST_NETWORK_NAME "KaZaA"

/* the username we loaded from the config file */
#define FST_USER_NAME (FST_PLUGIN->username)

/* max number of nodes we save in nodes file */
#define FST_MAX_NODESFILE_SIZE 1000

/* max number of results we want to be returned per search */
#define FST_MAX_SEARCH_RESULTS 0xFF

/* application strings for http */
#define FST_HTTP_SERVER "giFT-FastTrack " VERSION
#define FST_HTTP_AGENT "giFT-FastTrack " VERSION

/* use a participation level of > 1000 for downloading
 * the exact value fluctuates and cannot be determined exactly currently
 */
#define FST_DOWNLOAD_BOOST_PL

/* Our advertised bandwidth.
 * This tells the supernode how many users it should direct to us for
 * uploading.
 * It is a logarithmic scale.  0xd1 represents "infinity" (actually,
 * 1680 kbps).  The value is approximately 14*log_2(x)+59, where
 * x is the bandwidth in kbps.
 */
#define FST_ADVERTISED_BW 0x60

/* the amount of time we wait before retrying with another node 
 * after resolve/tcp_open failed
 */
#define FST_SESSION_NETFAIL_INTERVAL    (10*SECONDS)

/* timeout for sessions connects */
#define FST_SESSION_CONNECT_TIMEOUT		(8*SECONDS)

/* timeout for sessions handshakes */
#define FST_SESSION_HANDSHAKE_TIMEOUT	(10*SECONDS)

/* timeout for udp ping */
#define FST_UDP_DISCOVER_TIMEOUT        (8*SECONDS)

/* max number of simultaneous udp pings */
#define FST_UDP_DISCOVER_MAX_PINGS      (20)

/*****************************************************************************/

typedef struct
{
	Config *conf;					/* ~/.giFT/FastTrack/FastTrack.conf */
	char *username;					/* copy of user name from config file */

	FSTNodeCache *nodecache;		/* cache that holds known supernode
									 * addresses
									 */

	FSTIpSet *banlist;				/* set banned of ip ranges we loaded from
									 * ~/.giFT/FastTrack/banlist
									 */

	FSTHttpServer *server;			/* http server used for uploads,
									 * push replies and incoming sessions
									 */

	FSTSession *session;			/* established session to supernode we are
									 * currently using
									 */

	FSTUdpDiscover *discover;		/* pointer to udp node discovery object */

	FSTSearchList *searches;		/* list containing all currently running
									 * searches
									 */

	FSTStats *stats;				/* network statistics */

	FSTPushList *pushlist;			/* list of requested pushes */

	in_addr_t local_ip;				/* the ip our supernode connection is bound
									 * to locally
									 */

	in_addr_t external_ip;			/* our external ip as told by supernode */

	int forwarding;					/* if we're behind NAT and the user has
									 * set up port forwarding this is TRUE
									 * (from config file)
									 */

	int hide_shares;				/* TRUE if user has hidden shares.
									 * set to FALSE on startup
									 */

	int allow_sharing;				/* cache for allow_sharing config key */

} FSTPlugin;

/*****************************************************************************/

extern Protocol *fst_proto;			// global pointer to plugin struct

// called by gift to init plugin
#ifdef GIFT_PLUGIN_EXPORT
GIFT_PLUGIN_EXPORT
#endif
  int FastTrack_init (Protocol *p);

/*****************************************************************************/

#endif /* __FASTTRACK_H */

/*
 * $Id: fst_fasttrack.h,v 1.6 2003/06/20 22:29:17 beren12 Exp $
 *
 * Copyright (C) 2003 giFT-FastTrack project http://developer.berlios.de/projects/gift-fasttrack
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

#define FILE_LINE_FUNC __FILE__,__LINE__,__PRETTY_FUNCTION__

#define FST_DBG(fmt)				FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt)
#define FST_DBG_1(fmt,a)			FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a)
#define FST_DBG_2(fmt,a,b)			FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b)
#define FST_DBG_3(fmt,a,b,c)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c)
#define FST_DBG_4(fmt,a,b,c,d)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d)
#define FST_DBG_5(fmt,a,b,c,d,e)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d,e)

#ifdef HEAVY_DEBUG
# define FST_HEAVY_DBG(fmt)				FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt)
# define FST_HEAVY_DBG_1(fmt,a)			FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a)
# define FST_HEAVY_DBG_2(fmt,a,b)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b)
# define FST_HEAVY_DBG_3(fmt,a,b,c)		FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c)
# define FST_HEAVY_DBG_4(fmt,a,b,c,d)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d)
# define FST_HEAVY_DBG_5(fmt,a,b,c,d,e)	FST_PROTO->trace(FST_PROTO,FILE_LINE_FUNC,fmt,a,b,c,d,e)
#else
# define FST_HEAVY_DBG(fmt)
# define FST_HEAVY_DBG_1(fmt,a)
# define FST_HEAVY_DBG_2(fmt,a,b)
# define FST_HEAVY_DBG_3(fmt,a,b,c)
# define FST_HEAVY_DBG_4(fmt,a,b,c,d)
# define FST_HEAVY_DBG_5(fmt,a,b,c,d,e)
#endif

#define GIFT_PLUGIN
#include <libgift/libgift.h>

#include <libgift/giftconfig.h>

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

#include <stdint.h>

typedef int8_t   fst_int8;
typedef uint8_t  fst_uint8;
typedef int16_t  fst_int16;
typedef uint16_t fst_uint16;
typedef int32_t  fst_int32;
typedef uint32_t fst_uint32;

/*****************************************************************************/

#include <ctype.h>

#include <libgift/proto/protocol.h>
//#include <libgift/proto/if_event.h>
#include <libgift/file.h>
#include <libgift/parse.h>
#include <libgift/network.h>
#include <libgift/dataset.h>
#include <libgift/tcpc.h>

#if 0
#include "src/transfer.h"
#include "src/download.h" // download_remove_source
#endif

#include <libgift/proto/transfer_api.h>

#include <string.h>

#include <libgift/dataset.h>


#include "fst_node.h"
#include "fst_packet.h"
#include "fst_session.h"
#include "fst_hash.h"
#include "fst_search.h"
#include "fst_download.h"
#include "fst_stats.h"
#include "fst_utils.h"
#include "fst_meta.h"

/*****************************************************************************/

#define FST_PLUGIN ((FSTPlugin*)fst_proto->udata)
#define FST_PROTO (fst_proto)

#define FST_NETWORK_NAME "KaZaA"		// network name we send and which we require from other supernodes

#define FST_MAX_NODESFILE_SIZE 1000	// max number of nodes we save in nodes file

#define FST_MAX_SEARCH_RESULTS 0xFF	// max number of results we want to be returned per search

// various timeouts in ms
#define FST_SESSION_CONNECT_TIMEOUT		(10*SECONDS)
#define FST_SESSION_HANDSHAKE_TIMEOUT	(10*SECONDS)

#define FST_DOWNLOAD_CONNECT_TIMEOUT	(20*SECONDS)
#define FST_DOWNLOAD_HANDSHAKE_TIMEOUT	(10*SECONDS)
#define FST_DOWNLOAD_DATA_TIMEOUT		(10*SECONDS)

/*****************************************************************************/

typedef struct
{
	Config *conf;					// ~/.giFT/FastTrack/FastTrack.conf

	FSTNodeCache *nodecache;		// cache that holds known supernode addresses

	FSTSession *session;			// established session to supernode we're currently using

	FSTSearchList *searches;		// list containing all currently running searches

	FSTStats *stats;				// network statistics

} FSTPlugin;

/*****************************************************************************/

extern Protocol *fst_proto;			// global pointer to plugin struct

extern char *FST_USER_NAME;			// FastTrack Username from conf file

// called by gift to init plugin
#ifdef WIN32
int __declspec(dllexport) FastTrack_init (Protocol *p);
#else
int FastTrack_init (Protocol *p);
#endif

/*****************************************************************************/

#endif /* __FASTTRACK_H */

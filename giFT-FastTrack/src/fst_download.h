/*
 * $Id: fst_download.h,v 1.6 2003/09/10 11:10:25 mkern Exp $
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

#ifndef __FST_DOWNLOAD_H
#define __FST_DOWNLOAD_H

#include "fst_fasttrack.h"
#include "fst_http_client.h"

/*****************************************************************************/

/* called by gift to start downloading of a chunk */
int fst_giftcb_download_start (Protocol *p, Transfer *transfer, Chunk *chunk,
							   Source *source);

/* called by gift to stop download */
void fst_giftcb_download_stop (Protocol *p, Transfer *transfer, Chunk *chunk,
							   Source *source, int complete);

/* called by gift to add a source */
BOOL fst_giftcb_source_add (Protocol *p,Transfer *transfer, Source *source);

/* called by gift to remove source */
void fst_giftcb_source_remove (Protocol *p, Transfer *transfer,
							   Source *source);

/*****************************************************************************/

#endif /* __FST_DOWNLOAD_H */

# $Id: Makefile.am,v 1.6 2003/06/28 16:41:07 beren12 Exp $

SUBDIRS = crypt

AM_CFLAGS = $(FT_CFLAGS)

INCLUDES = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/crypt

plugin_LTLIBRARIES = libFastTrack.la

libFastTrack_la_SOURCES = \
	fst_download.c	fst_download.h \
	fst_fasttrack.c	fst_fasttrack.h \
	fst_hash.c	fst_hash.h \
	fst_http.c	fst_http.h \
	fst_meta.c	fst_meta.h \
	fst_node.c	fst_node.h \
	fst_packet.c	fst_packet.h \
	fst_search.c	fst_search.h \
	fst_session.c	fst_session.h \
	fst_stats.c	fst_stats.h \
	fst_utils.c	fst_utils.h \
	md5.c		md5.h

libFastTrack_la_LIBADD = \
	$(prefix)/lib/libgift.la \
	$(prefix)/lib/libgiftproto.la \
	$(top_builddir)/src/crypt/libfst_crypt.la

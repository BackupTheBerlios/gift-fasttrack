# $Id: Makefile.am,v 1.16 2003/09/18 14:54:50 mkern Exp $
AM_CFLAGS = -Wall

SUBDIRS = crypt

plugin_LTLIBRARIES = libFastTrack.la

libFastTrack_la_SOURCES = \
	fst_download.c		fst_download.h \
	fst_fasttrack.c		fst_fasttrack.h \
	fst_hash.c			fst_hash.h \
	fst_http_header.c	fst_http_header.h \
	fst_http_client.c	fst_http_client.h \
	fst_http_server.c	fst_http_server.h \
	fst_meta.c			fst_meta.h \
	fst_node.c			fst_node.h \
	fst_packet.c		fst_packet.h \
	fst_search.c		fst_search.h \
	fst_session.c		fst_session.h \
	fst_stats.c			fst_stats.h \
	fst_utils.c			fst_utils.h \
	fst_ipset.c			fst_ipset.h \
	fst_push.c			fst_push.h \
	md5.c				md5.h

libFastTrack_la_LIBADD = \
	$(top_builddir)/src/crypt/libfst_crypt.la

INCLUDES = \
	@GIFT_CFLAGS@ \
	-I$(top_srcdir)/src/crypt

libFastTrack_la_LDFLAGS = @GIFT_LIBS@ -module -avoid-version -Wl,-z,defs

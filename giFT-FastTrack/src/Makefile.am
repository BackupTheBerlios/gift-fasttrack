# $Id: Makefile.am,v 1.8 2003/06/29 13:36:07 beren12 Exp $

SUBDIRS = crypt

plugin_LTLIBRARIES = libFastTrack.la

libFastTrack_la_SOURCES =	\
	fst_download.c		\
	fst_fasttrack.c		\
	fst_hash.c		\
	fst_http.c		\
	fst_meta.c		\
	fst_node.c		\
	fst_packet.c		\
	fst_search.c		\
	fst_session.c		\
	fst_stats.c		\
	fst_utils.c		\
	md5.c

libFastTrack_la_LIBADD = \
	$(prefix)/lib/libgift.la \
	$(prefix)/lib/libgiftproto.la \
	$(top_builddir)/src/crypt/libfst_crypt.la

INCLUDES = \
	@FT_CFLAGS@ \
	-I$(top_srcdir)/src/crypt


libFastTrack_la_LDFLAGS = @FT_LDFLAGS@ @FT_LIBS@ -lm

# $Id: Makefile.am,v 1.6 2003/07/10 13:08:20 weinholt Exp $

noinst_LTLIBRARIES = libfst_crypt.la

libfst_crypt_la_SOURCES = \
	enc_type_1.c \
	enc_type_2.c \
	enc_type_20.c \
	fst_crypt.c fst_crypt.h

INCLUDES = \
	@FT_CFLAGS@ \
	-I$(top_srcdir)/src

libfst_crypt_la_LDFLAGS = @FT_LDFLAGS@ @FT_LIBS@ -lm

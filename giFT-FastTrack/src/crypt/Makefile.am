# $Id: Makefile.am,v 1.3 2003/06/29 13:15:00 beren12 Exp $

noinst_LTLIBRARIES = libfst_crypt.la

libfst_crypt_la_SOURCES = \
	enc_type_1.c \
	enc_type_2.c \
	enc_type_20.c \
	fst_crypt.c

INCLUDES = @FT_CFLAGS@

libfst_crypt_la_LDFLAGS = @FT_LDFLAGS@ @FT_LIBS@ -lm

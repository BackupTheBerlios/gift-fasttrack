# $Id: Makefile.am,v 1.7 2003/07/10 17:53:19 weinholt Exp $

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

TESTS = crypt_test
check_PROGRAMS = crypt_test
crypt_test_SOURCES = crypt_test.c
crypt_test_LDADD = ../md5.o enc_type_1.o enc_type_2.o enc_type_20.o -lm

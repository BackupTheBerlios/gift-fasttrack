# $Id: Makefile.am,v 1.8 2003/07/11 14:47:23 weinholt Exp $

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
crypt_test_LDADD = ../md5.o enc_type_1.lo enc_type_2.lo enc_type_20.lo -lm

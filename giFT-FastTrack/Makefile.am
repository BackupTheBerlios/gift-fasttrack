# $Id: Makefile.am,v 1.11 2003/12/26 12:00:26 mkern Exp $

SUBDIRS = src data m4

EXTRA_DIST = \
	fasttrack.spec \
	Make.msvc \
	Makefile.msvc

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz

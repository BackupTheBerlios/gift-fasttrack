# $Id: Makefile.am,v 1.9 2003/09/19 14:46:26 mkern Exp $

SUBDIRS = src data m4
EXTRA_DIST = fasttrack.spec

rpm : dist
      rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz

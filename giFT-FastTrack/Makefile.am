# $Id: Makefile.am,v 1.10 2003/09/19 18:26:41 weinholt Exp $

SUBDIRS = src data m4
EXTRA_DIST = fasttrack.spec

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz

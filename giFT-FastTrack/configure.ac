# -*- sh -*-

# $Id: configure.ac,v 1.17 2003/07/19 16:30:28 weinholt Exp $

dnl ###########################################################################
dnl Process this file with autoconf to produce a configure script.
dnl ###########################################################################

AC_PREREQ(2.52)

AC_INIT(giFT-FastTrack, 0.8.2, gift-fasttrack-devel@lists.berlios.de, giFT-FastTrack)
AC_REVISION($Revision: 1.17 $)
AC_CONFIG_SRCDIR([src/fst_fasttrack.h])

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

dnl AM_MAINTAINER_MODE

dnl ###########################################################################
dnl Build information
dnl ###########################################################################

AC_DEFINE_UNQUOTED(BUILD_DATE, "`date`", [Build date])

dnl ###########################################################################
dnl Checks for programs.
dnl ###########################################################################

AC_PROG_CC
AC_HEADER_STDC
AC_PROG_INSTALL

AM_DISABLE_STATIC
AC_PROG_LIBTOOL

dnl ###########################################################################
dnl Checks for libraries.
dnl ###########################################################################

OLD_PKG_CONFIG_PATH=$PKG_CONFIG_PATH
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
PKG_CHECK_MODULES(GIFT, libgift >= 0.10.0)
PKG_CONFIG_PATH=$OLD_PKG_CONFIG_PATH

dnl ###########################################################################
dnl Checks for header files.
dnl ###########################################################################

AC_HEADER_STDC

dnl ###########################################################################
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ###########################################################################

AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM

dnl ###########################################################################
dnl Checks for library functions.
dnl ###########################################################################

AC_FUNC_MEMCMP
AC_CHECK_FUNCS(strdup)

dnl ###########################################################################
dnl All of the lovely arguments
dnl ###########################################################################

AC_ARG_ENABLE(debug,
  AC_HELP_STRING([--disable-debug], [do not create debug messages]),
  [], [enable_debug=yes])

AC_ARG_ENABLE(heavy_debug,
  AC_HELP_STRING([--enable-heavy-debug], [create _lots_ debug messages]),
  [], [enable_heavy_debug=no])

dnl ###########################################################################
dnl Architecture specific stuff
dnl ###########################################################################

# AC_CANONICAL_HOST

case "${host}" in
  *-*-linux* )
    LIBS="$LIBS -rdynamic"
  ;;
  *-*-darwin* )
    dnl PowerPC & x86 Darwin based distributions (including Mac OS X)
    LIBS="$LIBS -dynamic"
  ;;
esac

dnl ###########################################################################
dnl normal debug
dnl ###########################################################################

use_debug=no
if test x$enable_debug = xyes; then
	AC_DEFINE(DEBUG, 1, [Enable debugging])
	use_debug=yes
fi

dnl ###########################################################################
dnl heavy debug
dnl ###########################################################################

use_heavy_debug=no
if test x$enable_heavy_debug = xyes; then
	AC_DEFINE(HEAVY_DEBUG, 1, [Enable heavy debugging])
	use_heavy_debug=yes
fi

dnl ###########################################################################
dnl Setup some misc build environment settings
dnl ###########################################################################

if test x$prefix = xNONE; then
    prefix=`pkg-config libgift --variable=prefix`
    AC_SUBST(prefix)
fi

plugindir=${prefix}/lib/giFT
AC_SUBST(plugindir)

datadir=${prefix}/share/giFT
AC_SUBST(datadir)

dnl ###########################################################################
dnl Finally do the generation
dnl ###########################################################################

dnl Needed to build with libtoolize 1.4.3 on Debian woody
AC_CONFIG_AUX_DIR(.)

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/crypt/Makefile
  data/Makefile
])
AC_OUTPUT

dnl ###########################################################################
dnl Print summary to the user
dnl ###########################################################################

echo
echo "$PACKAGE $VERSION"
echo
echo "------------------ core ------------------"
echo
echo "normal debugging........: $use_debug"
echo "heavy debugging.........: $use_heavy_debug"
echo
echo "------------------------------------------"
echo
echo "$PACKAGE will be installed in $plugindir/"
echo "configs will be installed in $datadir/"
echo

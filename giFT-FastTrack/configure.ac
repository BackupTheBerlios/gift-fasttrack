# -*- sh -*-

# $Id: configure.ac,v 1.7 2003/06/20 20:15:30 beren12 Exp $

dnl ###########################################################################
dnl Process this file with autoconf to produce a configure script.
dnl ###########################################################################

AC_PREREQ(2.52)

AC_INIT
AC_CONFIG_SRCDIR([src/fst_fasttrack.h])

AM_INIT_AUTOMAKE(giFT-FastTrack, 0.0.1)
AM_CONFIG_HEADER(config.h)

dnl AM_MAINTAINER_MODE

dnl ###########################################################################
dnl acconfig.h
dnl ###########################################################################

AH_VERBATIM([BUILD_DATE], [
#undef BUILD_DATE
])
AH_VERBATIM([USE_LTDL], [
#undef USE_LTDL
])

dnl ###########################################################################
dnl Build information
dnl ###########################################################################

BUILD_DATE="`date`"
AC_DEFINE_UNQUOTED(BUILD_DATE, "$BUILD_DATE")

dnl ###########################################################################
dnl Checks for programs.
dnl ###########################################################################

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_PROG_INSTALL

AM_DISABLE_STATIC
AC_PROG_LIBTOOL

dnl ###########################################################################
dnl Checks for libraries.
dnl ###########################################################################

PKG_CHECK_MODULES(FT, libgift >= 0.10.0)

dnl ###########################################################################
dnl Checks for header files.
dnl ###########################################################################

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT

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
AC_FUNC_MMAP
AC_CHECK_FUNCS(strdup)

dnl ###########################################################################
dnl All of the lovely arguments
dnl ###########################################################################

AC_ARG_ENABLE(ltdl,        [  --disable-ltdl          do not use libltdl],,[enable_ltdl=yes])
AM_CONDITIONAL(LTDL,        test x$enable_ltdl    = xyes)

dnl ###########################################################################
dnl Overrides
dnl ###########################################################################

DL_MODULE=
DL_LIBS=
DL_LDFLAGS=
DL_CFLAGS=

# Override -g -O2
CFLAGS="-g -Wall"

dnl ###########################################################################
dnl Architecture specific stuff
dnl ###########################################################################

# AC_CANONICAL_HOST is already REQUIRED by libtool, i think
#AC_CANONICAL_HOST

PROGRAM_EXT=
SO_SUFFIX=so
ZLIB_VER=1.1.4

case "${host}" in
  *-hp-hpux* )
    SO_SUFFIX=sl
  ;;
  *-*-linux* )
    DL_LDFLAGS='-rdynamic'
  ;;
  *-*-cygwin*)
    SO_SUFFIX=dll
    PROGRAM_EXT=".exe"
  ;;

  *-*-darwin* )
    dnl PowerPC & x86 Darwin based distributions (including Mac OS X)
    SO_SUFFIX=dylib
    ZLIB_VER=1.1.3
    DL_CFLAGS="-Ddlsym=dlsym_prepend_underscore"
    DL_LDFLAGS="-dynamic"
  ;;
esac

dnl ###########################################################################
dnl libltdl checks
dnl ###########################################################################

# set the defaults
use_ltdl=no
FT_CFLAGS="$FT_CFLAGS $CFLAGS"
FT_LIBS="$FT_LIBS $LIBS \$(top_builddir)/src/libFastTrack.la"

if test x$enable_ltdl = xyes; then
    dnl check for lt_dlopen and lt_dlsym in libltdl
    AC_CHECK_LIB(ltdl, lt_dlopen,
        [AC_CHECK_LIB(ltdl, lt_dlsym, found_libltdl=yes, found_libltdl=no )] ,)

    dnl check for ltdl header files
    AC_CHECK_HEADERS(ltdl.h,
            [ found_ltdlh=yes ],
            [ found_ltdlh=no ])

    # only add flags for ltdl stuff if all the tests passed
    if (test x$found_libltdl = xyes) && (test x$found_ltdlh = xyes); then
        use_ltdl=yes
        DL_LIBS=-lltdl
        FT_LIBS="$LIBS $DL_LIBS"
        FT_LDFLAGS="$LDFLAGS $DL_LDFLAGS"
        FT_CFLAGS="$FT_CFLAGS $DL_CFLAGS"

        AC_DEFINE(USE_LTDL)
        AC_SUBST(USE_LTDL)
    fi
fi

dnl ###########################################################################
dnl Setup some misc build environment settings
dnl ###########################################################################

AC_SUBST(FT_CFLAGS)
AC_SUBST(FT_LIBS)
AC_SUBST(FT_LDFLAGS)

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

AC_CONFIG_FILES([
Makefile
src/Makefile
data/Makefile
])
AC_OUTPUT

dnl ###########################################################################
dnl Print summary to the user
dnl ###########################################################################

echo
echo "$PACKAGE $VERSION"
echo
echo "------------ core ------------"
echo
echo "use ltdl................: $use_ltdl"
echo
echo "-----------------------------"
echo
echo "$PACKAGE will be installed in $plugindir/"
echo " configs will be installed in $datadir/"
echo
echo

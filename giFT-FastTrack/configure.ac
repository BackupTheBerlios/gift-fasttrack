# -*- sh -*-

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

AC_CHECK_LIB(nsl, gethostbyname)
AC_CHECK_LIB(socket, socket)
AC_CHECK_LIB(mingwex, opendir)
AC_CHECK_LIB(bind, inet_ntoa)
AC_CHECK_LIB(be, openlog)
AC_CHECK_LIB(gift, libgift_init)

dnl ###########################################################################
dnl Checks for header files.
dnl ###########################################################################

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(dirent.h \
                 fcntl.h \
                 getopt.h \
                 limits.h \
                 linux/limits.h \
                 sys/mman.h \
                 sys/time.h \
                 syslog.h \
                 signal.h \
                 unistd.h \
                 inttypes.h \
                 io.h \
                 libgift/libgift.h \
)

dnl ###########################################################################
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ###########################################################################

AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM

AS_IF([test "${ac_cv_header_time}" = yes],
      [AC_DEFINE(TIME_WITH_SYS_TIME)],
      [])

AS_IF([test "${ac_cv_header_sys_time_h}" = yes],
      [AC_DEFINE(HAVE_SYS_TIME_H)],
      [])

AS_IF([test "${ac_cv_struct_tm}" = sys/time.h],
      [AC_DEFINE(TM_IN_SYS_TIME)],
      [])

AC_C_BIGENDIAN

AS_IF([test "${ac_cv_c_bigendian}" = yes],
      [AC_DEFINE(WORDS_BIGENDIAN)],
      [])

AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(void *)

AC_DEFINE_UNQUOTED(SIZEOF_SHORT, AS_VAR_GET(ac_cv_sizeof_short))
AC_DEFINE_UNQUOTED(SIZEOF_INT, AS_VAR_GET(ac_cv_sizeof_int))
AC_DEFINE_UNQUOTED(SIZEOF_LONG, AS_VAR_GET(ac_cv_sizeof_long))

if test "${ac_cv_sizeof_int}" = "4" ; then
   AS_VAR_SET(ac_cv_gift_int32, int)
elif test "${ac_cv_sizeof_long}" = "4"; then
   AS_VAR_SET(ac_cv_gift_int32, long)
else
   AC_MSG_ERROR(No 32-bit datatype found - please report this to the developers)
fi

AC_CHECK_TYPE(uint8_t, unsigned char)
AC_CHECK_TYPE(int8_t, char)
AC_CHECK_TYPE(uint16_t, unsigned short)
AC_CHECK_TYPE(int16_t, short)
AC_CHECK_TYPE(uint32_t, unsigned AS_VAR_GET(ac_cv_gift_int32))
AC_CHECK_TYPE(int32_t, AS_VAR_GET(ac_cv_gift_int32))

dnl TODO, check for _Bool from C99 or bool from some other whacky platform
AC_DEFINE(BOOL, int, [Define BOOL])
AC_DEFINE(TRUE, 1, [TRUE=1])
AC_DEFINE(FALSE, 0, [FASLE=0])

AC_CHECK_TYPE(in_addr_t, uint32_t,
                   [Define to 'uint32_t' if <netinet/in.h> and <sys/types.h> do not define],
                   [#include <sys/types.h>
                    #include <netinet/in.h>
                    ])

AC_CHECK_TYPE(in_port_t, uint16_t,
                   [Define to 'uint16_t' if <netinet/in.h> and <sys/types.h> do not define],
                   [#include <sys/types.h>
                    #include <netinet/in.h>
                    ])

AC_CHECK_TYPE(pid_t, int)
AC_CHECK_TYPE(off_t, long)
AC_CHECK_TYPE(size_t, unsigned)


dnl ###########################################################################
dnl Checks for library functions.
dnl TODO: Wrap these in m4/giftconfig.m4
dnl ###########################################################################

AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_CHECK_FUNCS(ftruncate madvise mkstemp nice select poll signal socket socketpair strdup strstr)
AC_CHECK_FUNCS(snprintf _snprintf vsnprintf _vsnprintf)

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
FT_CFLAGS="$CFLAGS"
FT_LIBS="$LIBS \$(top_builddir)/src/libFastTrack.la"

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
        FT_CFLAGS="$CFLAGS $DL_CFLAGS"

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
    prefix="/usr/local"
    AC_SUBST(prefix)
fi

libgiftincdir=${prefix}/include/libgift
AC_SUBST(libgiftincdir)

AC_DEFUN([FT_HEADER],
         [AS_IF([test "${ac_cv_header_$1}" = yes],
                [AC_DEFINE([$2])],
                [])
          ])

dnl TODO: wrap AC_CHECK_HEADERS usage above
FT_HEADER(stdint_h, HAVE_STDINT_H)
FT_HEADER(inttypes_h, HAVE_INTTYPES_H)
FT_HEADER(unistd_h, HAVE_UNISTD_H)
FT_HEADER(dirent_h, HAVE_DIRENT_H)
FT_HEADER(dlfcn_h, HAVE_DLFCN_H)
FT_HEADER(fcntl_h, HAVE_FCNTL_H)
FT_HEADER(io_h, HAVE_IO_H)
FT_HEADER(stdlib_h, HAVE_STDLIB_H)
FT_HEADER(strings_h, HAVE_STRINGS_H)
FT_HEADER(syslog_h, HAVE_SYSLOG_H)
FT_HEADER(sys_dir_h, HAVE_SYS_DIR_H)
FT_HEADER(sys_mman_h, HAVE_SYS_MMAN_H)
FT_HEADER(sys_ndir_h, HAVE_SYS_NDIR_H)
FT_HEADER(sys_stat_h, HAVE_SYS_STAT_H)
FT_HEADER(sys_types_h, HAVE_SYS_TYPES_H)
FT_HEADER(limits_h, HAVE_LIMITS_H)
FT_HEADER(linux_limits_h, HAVE_LINUX_LIMITS_H)
FT_HEADER(memory_h, HAVE_MEMORY_H)
FT_HEADER(ndir_h, HAVE_NDIR_H)

dnl ###########################################################################
dnl Finally do the generation
dnl ###########################################################################

AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT

dnl ###########################################################################
dnl Print summary to the user
dnl ###########################################################################

echo
echo "$PACKAGE $VERSION"
echo
echo "-------------------- core ---"
echo
echo "use ltdl................: $use_ltdl"
echo
echo "-----------------------------"
echo
echo "$PACKAGE will be installed in $plugindir/"
echo
echo

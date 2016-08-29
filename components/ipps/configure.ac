#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(ipps, 1.0.0, tsumardi@gmail.com)
AM_INIT_AUTOMAKE(ipps, 1.0.0)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
#AC_CONFIG_LIBOBJ_DIR([lib])
#AC_LIBOBJ([obj])
AC_PROG_RANLIB
PKG_PROG_PKG_CONFIG
AC_CONFIG_MACRO_DIR([m4])

PKG_CHECK_MODULES([glib2], [glib-2.0 >= 2.42])

#check package/shared library dependencies
#AC_CHECK_PROG(PFRING_CHECK,pfring,yes)
#if test x"$PFRING_CHECK" != x"yes" ; then
#   AC_MSG_ERROR([Please install pfring.])
#fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 man/Makefile
                 scripts/Makefile
                 src/Makefile
                 src/bkgrnd/Makefile
                 src/logging/Makefile
                 src/main/Makefile
                 src/pfilter/Makefile
                 src/thdmgmt/Makefile])

AC_OUTPUT

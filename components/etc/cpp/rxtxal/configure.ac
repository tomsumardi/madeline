#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(rxtxal, 1.0.0, tsumardi@gmail.com)
AM_INIT_AUTOMAKE(rxtxal, 1.0.0)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
#AC_CONFIG_LIBOBJ_DIR([lib])
#AC_LIBOBJ([obj])
AC_PROG_RANLIB
PKG_PROG_PKG_CONFIG
AC_CONFIG_MACRO_DIR([m4])

AC_SUBST(CPPFLAGS, "-I ../../../cpp")

PKG_CHECK_MODULES([glib2], [glib-2.0 >= 2.42])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/foreach.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 man/Makefile
                 scripts/Makefile
                 src/Makefile])

AC_OUTPUT

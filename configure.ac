#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([WarGS], [0.1], [wargs@googlegroups.com])
AC_CONFIG_SRCDIR([src/wargs.c])
AC_CONFIG_HEADERS([src/config.h])

AM_INIT_AUTOMAKE([gnits -Wall -Werror])

# Checks for programs.
AC_PROG_CC
AM_PROG_AS
PKG_PROG_PKG_CONFIG
AM_PATH_PYTHON

# Checks for libraries.
PKG_CHECK_MODULES([LIBCONFIG], [libconfig])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

CFLAGS="-Wall -Werror ${CFLAGS} ${LIBCONFIG_CFLAGS}"
LIBS="${LIBS} ${LIBCONFIG_LIBS}"

MKWRAPPERS_FLAGS="-g -s 4"
AC_SUBST([MKWRAPPERS_FLAGS])

AC_DEFINE([FILEPATH_SEPARATOR], ['/'], [File path separator.])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

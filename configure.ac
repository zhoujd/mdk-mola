# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([zzmediaxapp], [1.0], [zachary.zhou@hotmail.com])
AC_CONFIG_SRCDIR([Makefile.am])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([config.h])
AC_PROG_RANLIB


# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([LIBVA], [libva])
PKG_CHECK_MODULES([LIBDRM], [libdrm])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.


AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo
echo "zzmediaxapp - ${VERSION}"
echo
echo

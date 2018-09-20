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

AC_ARG_ENABLE(libudev_use,
    [AC_HELP_STRING([--enable-libudev_use],
                    [build use libudev-dev @<:@default=yes@:>@])],
    [], [enable_libudev_use="yes"])

# Checks for libraries.
#if test x$enable_libudev_use = xyes; then
#   PKG_CHECK_MODULES([UDEV], [libudev], [libudev=yes], [libudev=no])
#   if test "$libudev" = "yes"; then
#       AC_DEFINE([HAVE_LIBUDEV_DEV], [1], [Defined to 1 if libudev-dev API is built])
#       LIBS="$LIBS -ludev"
#   fi
#fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.


AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo
echo "zzmediaxapp - ${VERSION}"
echo
echo Use libudev-dev .............. : $enable_libudev_use
echo

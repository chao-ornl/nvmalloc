# -*- shell-script -*-
#
# Copyright 2017        UT-Battelle, LLC.
#                       All rights reserved.
# See COPYING in top-level directory.
# 
# Additional copyrights may follow
# 
# $HEADER$
#

AC_INIT([nvm], [0.1], [valleegr@ornl.gov])
AC_CONFIG_SRCDIR([src/nvm.h])

AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects foreign tar-ustar])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_MACRO_DIR([m4])

CFLAGS="$CFLAGS -std=c90 -fPIC"

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AM_PROG_CC_C_O
m4_pattern_allow([AM_PROG_AR], [AM_PROG_AR])
LT_INIT

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT

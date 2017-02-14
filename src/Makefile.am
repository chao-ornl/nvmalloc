#
# Copyright 2017        UT-Battelle, LLC
#                       All rights reserved.
#
# See COPYING in top-level directory.
# 
# Additional copyrights may follow
# 
# $HEADER$
#

bin_PROGRAMS =

COMMON_SOURCES =            \
        nvm.c               \
        ssd_device.c        \
        namespace.c

headers = nvm.h

include_HEADERS = $(headers)

lib_LTLIBRARIES = libnvm.la
libnvm_la_SOURCES = $(COMMON_SOURCES) $(headers)
libnvm_la_LIBADD =

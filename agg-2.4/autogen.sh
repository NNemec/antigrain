#!/bin/sh

# autogen.sh
#
# invoke the auto* tools to create the configuration/build system

# build aclocal.m4
aclocal

# build config.h
autoheader

# build the configure script
autoconf

# set up libtool
if which glibtoolize ; then
    # OSX homebrew has glibtoolize but no libtoolize
    glibtoolize --copy --force
else
    libtoolize --copy --force
fi

# invoke automake
automake --copy --foreign --add-missing --ignore-deps

# and finally invoke our new configure
#./configure $*

# end

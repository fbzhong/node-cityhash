#!/bin/sh
cd `dirname $0`

src="cityhash/src"

# get the cityhash src if needed.
if [ ! -d "${src}" ] ; then
    mkdir -p "${src}"
fi

if [ ! -e "${src}/city.h" ] ; then
    wget -O "${src}/city.h" http://cityhash.googlecode.com/svn/trunk/src/city.h
fi

if [ ! -e "${src}/city.cc" ] ; then
    wget -O "${src}/city.cc" http://cityhash.googlecode.com/svn/trunk/src/city.cc
fi

# build.
node-waf configure
node-waf build

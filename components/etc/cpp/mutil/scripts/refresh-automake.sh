#!/bin/bash

set -x

cd ../
#libtoolize --force
autoscan
#diff configure.ac configure.scan
aclocal
autoheader 
autoconf
automake --force-missing --add-missing
cd scripts

#!/bin/bash

set -x

cd ../
autoscan
diff configure.ac configure.scan
aclocal
autoheader 
autoconf
automake --add-missing
cd scripts

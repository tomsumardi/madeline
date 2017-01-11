#!/bin/bash
cd ../
make maintainer-clean
find ./ -name Makefile.in | xargs rm
cd scripts

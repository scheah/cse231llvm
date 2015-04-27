#!/bin/bash

filename=$(basename "$1")
opt -analyze -load $LLVMLIB/CSE231.so -o $OUTPUTLOGS/$filename.static.log -count_static < $1/$filename.bc > /dev/null

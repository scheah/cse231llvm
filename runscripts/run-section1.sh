#!/bin/bash

filename=$(basename "$f")
opt -analyze -load $LLVMLIB/CSE231.so -o $OUTPUTLOGS/$filename.static.log -count_static < $f > /dev/null

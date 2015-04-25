#!/bin/bash
FILES=$1/*.bc
for f in $FILES
do
	filename=$(basename "$f")
	filename="${filename%.*}"
	opt -analyze -load $LLVMLIB/CSE231.so -o $OUTPUTLOGS/$filename.static.log -count_static < $f > /dev/null
done
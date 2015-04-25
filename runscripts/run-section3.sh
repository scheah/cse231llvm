#!/bin/bash

CPPFLAGS=
LLVMLIBS=
LDFLAGS=
# if your instrumentation code calls into LLVM libraries, then comment out the above and use these instead:
#CPPFLAGS=`llvm-config --cppflags`
#LLVMLIBS=`llvm-config --libs`
#LDFLAGS=`llvm-config --ldflags`

filename=$(basename "$1")
filename="${filename%.*}"
opt -load $LLVMLIB/CSE231.so -branchbias < $1/$filename.bc > $CSE231ROOT/$filename.pass.bc
## compile the instrumentation module to bitcode
clang $CPPFLAGS -O0 -emit-llvm -c $INSTRUMENTATION/branchbias/BranchBiasInstrumentation.cpp -o $INSTRUMENTATION/branchbias/BranchBiasInstrumentation.bc

## link instrumentation module
llvm-link $CSE231ROOT/$filename.pass.bc $INSTRUMENTATION/branchbias/BranchBiasInstrumentation.bc -o $INSTRUMENTATION/branchbias/$filename.linked.bc

## compile to native object file
llc -filetype=obj $INSTRUMENTATION/branchbias/$filename.linked.bc -o=$INSTRUMENTATION/branchbias/$filename.o

	## generate native executable
g++ $INSTRUMENTATION/branchbias/$filename.o $LLVMLIBS $LDFLAGS -o $INSTRUMENTATION/branchbias/$filename.exe

$INSTRUMENTATION/branchbias/$filename.exe > $OUTPUTLOGS/$filename.branchbias.log


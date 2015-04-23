#include <stdio.h>
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

int Record()//BasicBlock &block)
{
    outs() << "blah blah\n";
    errs() << "yes\n";
    printf("Hello");
    return 0;
}

extern void PrintInstructionCount()
{
}


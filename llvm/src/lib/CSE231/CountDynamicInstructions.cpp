#define DEBUG_TYPE "hello"

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
    class DynamicAnalysisPass : public BasicBlockPass {

public:
        static char ID;
        DynamicAnalysisPass() : BasicBlockPass(ID) {}

        virtual bool runOnBasicBlock(BasicBlock &B) {
            if (B.getTerminator() == NULL) {
                errs() << "no terminator" << '\n';
                return false;
            }
            errs() << "terminator\n";

            Module * module = new Module("Current module", getGlobalContext());
            Constant * constant = module->getOrInsertFunction("Record", IntegerType::get(getGlobalContext(),32), Type::getLabelTy(getGlobalContext()), NULL);
            Function * function = cast<Function>(constant);

            IRBuilder<> irBuilder(&B);
            TerminatorInst * terminator = B.getTerminator();

            if (terminator != NULL)
                irBuilder.SetInsertPoint(terminator);
            else
                irBuilder.SetInsertPoint(&B);
            
            irBuilder.CreateCall(function, &B);

            return false;
        }
    };
}

char DynamicAnalysisPass::ID = 1;
static RegisterPass<DynamicAnalysisPass> X("dynamic", "Dynamic Analysis Pass");


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
#include <map>
#include <string.h>

using namespace llvm;
using namespace std;

namespace {
	struct DynamicAnalysisPass : public BasicBlockPass {
		static char ID;
		DynamicAnalysisPass() : BasicBlockPass(ID) {}

		virtual bool runOnBasicBlock(BasicBlock &B) {
			if (B.getTerminator() == NULL) {
				errs() << "no terminator" << '\n';
				return false;
			}
			bool returnStatementFound = false;
			map<const char*, int> instCounter;
			for (BasicBlock::iterator I = B.begin(); I != B.end(); ++I) { 
				instCounter[I->getOpcodeName()]++;
				if(ReturnInst::classof(I)) 
					returnStatementFound = true;
			}
			Module * module = B.getParent()->getParent();
			Function * function = B.getParent();
			Constant * recordConstant = module->getOrInsertFunction("_Z6RecordPci", FunctionType::getVoidTy(B.getContext()), PointerType::getInt8PtrTy(B.getContext()), IntegerType::get(B.getContext(),32), NULL);
			Function * recordFunction = cast<Function>(recordConstant);
			Constant * printConstant = module->getOrInsertFunction("_Z21PrintInstructionCountv", FunctionType::getVoidTy(B.getContext()), NULL);
			Function * printFunction = cast<Function>(printConstant);

			IRBuilder<> irBuilder(&B);
			TerminatorInst * terminator = B.getTerminator();

			if (terminator != NULL)
				irBuilder.SetInsertPoint(terminator);
			else
				irBuilder.SetInsertPoint(&B);
			for(map<const char*, int>::const_iterator iterator = instCounter.begin(); iterator != instCounter.end(); ++iterator) {
				Value *arg1 = irBuilder.CreateGlobalStringPtr(iterator->first);
				Value *arg2 = irBuilder.getInt32(iterator->second);
				irBuilder.CreateCall2(recordFunction, arg1, arg2);
			}
			if (strcmp(function->getName().str().c_str(), "main") == 0 && returnStatementFound) {
				irBuilder.CreateCall(printFunction);
			}
			return true;
		}
    	};
}

char DynamicAnalysisPass::ID = 0;
static RegisterPass<DynamicAnalysisPass> X("dynamic", "Dynamic Analysis Pass");


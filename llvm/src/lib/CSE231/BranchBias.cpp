#define DEBUG_TYPE "BranchBias"
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
	struct BranchBiasPass : public BasicBlockPass {
		static char ID;

		BranchBiasPass() : BasicBlockPass(ID) {}

		virtual bool runOnBasicBlock(BasicBlock &B) {
			Function * function = B.getParent();
			Module * module = function->getParent();
			Constant * recordConstant = module->getOrInsertFunction("_Z6RecordPci", FunctionType::getVoidTy(B.getContext()), PointerType::getInt8PtrTy(B.getContext()), IntegerType::get(B.getContext(),1), NULL);
			Function * recordFunction = cast<Function>(recordConstant);
			Constant * printConstant = module->getOrInsertFunction("_Z15PrintBranchBiasv", FunctionType::getVoidTy(B.getContext()), NULL);
			Function * printFunction = cast<Function>(printConstant);			

			TerminatorInst * terminatorInst = B.getTerminator();
			bool finalMainBlock = strcmp(function->getName().str().c_str(), "main") == 0 && ReturnInst::classof(terminatorInst);
			bool branchInstPresent = BranchInst::classof(terminatorInst);
			if (!finalMainBlock && !branchInstPresent) {
				return false;
			}
			IRBuilder<> irBuilder(&B);
			irBuilder.SetInsertPoint(terminatorInst);

			if (branchInstPresent) {
				BranchInst * branchInst = (BranchInst *)terminatorInst;
				if (branchInst->isConditional()) {
					Value *arg1 = irBuilder.CreateGlobalStringPtr(function->getName().str().c_str());
					Value *arg2 = branchInst->getCondition();
					irBuilder.CreateCall2(recordFunction, arg1, arg2);
				}
			}

			if (finalMainBlock) {
				irBuilder.CreateCall(printFunction);
			}

			return true;
		}
	};
}

char BranchBiasPass::ID = 0;
static RegisterPass<BranchBiasPass> X("branchbias", "Branch Bias Analysis Pass");

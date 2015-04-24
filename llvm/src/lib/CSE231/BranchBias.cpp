#define DEBUG_TYPE "CountDynamicInstructions"

namespace {
    struct BranchBiasPass : public BasicBlockPass {
        static char ID;
        
        BranchBiasPass() : BasicBlockPass(ID) {}

        virtual bool runOnBasicBlock(BasicBlock &B) {
            TerminatorInst * terminatorInst = B.getTerminator();

            if (terminatorInst == NULL || !TerminatorInst::classof(BranchInst)) {
                return false;
            }

            BranchInst * branchInst = (BranchInst *)terminatorInst;

            if (!branchInst->isConditional()) {
                return false;
            }

            Function * function = B.getParent();
            Module * = function->getParent();

            Constant * recordConstant = module->getOrInsertFunction("_Z6RecordPci", FunctionType::getVoidTy(B.getContext()), PointerType::getInt8PtrTy(B.getContext()), IntegerType::get(B.getContext(),32), NULL);
            Function * recordFunction = cast<Function>(recordConstant);
            Constant * printConstant = module->getOrInsertFunction("_Z15PrintBranchBiasv", FunctionType::getVoidTy(B.getContext()), NULL);
            Function * printFunction = cast<Function>(printConstant);

            IRBuilder<> irBuilder(&B);
            irBuilder.SetInsertPoint(terminatorInst);

            irBuilder.CreateCall2(recordFunction, arg1, arg2);

            if (strcmp(function->getName().str().c_str(), "main") == 0 && returnStatementFound) {
                irBuilder.CreateCall(printFunction);
            }

            return true;
        }
    }
}




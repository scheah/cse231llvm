//===- CountStaticInstructions.cpp - 				    ---------------===//
//
// Write a pass that counts the number of instructions in the program. 
// The pass should output the total number of instructions, as well as a per-instruction count. 
// For example, if there are 2 loads and 3 adds, then the output should be similar to this:
// 		load  2
// 		add   3
// 		TOTAL 5
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "CountStaticInstructions"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include <map>
#include <stdio.h>

using namespace llvm;
using namespace std;

namespace {
  // CountStaticInstructions - The first implementation, without getAnalysisUsage.
	struct CountStaticInstructions : public ModulePass {
		static char ID; // Pass identification, replacement for typeid
		map<const char*, int> instCounter;
		
		CountStaticInstructions() : ModulePass(ID) {}

		virtual bool runOnModule(Module &M) {
			printf("Hello\n");
			for (Module::iterator F = M.begin(); F != M.end(); ++F) { 
				for (Function::iterator B = F->begin(); B != F->end(); ++B) { 
					for (BasicBlock::iterator I = B->begin(); I != B->end(); ++I) { 
						instCounter[I->getOpcodeName()]++;
					}
				}
			}
			return false; // not modifying the module
		}
		virtual void print(raw_ostream&	O, const Module* M) const {
			int sum = 0;
			for(map<const char*, int>::const_iterator iterator = instCounter.begin(); iterator != instCounter.end(); ++iterator) {
				O() << iterator->first << "\t" << iterator->second << "\n";
				sum += iterator->second;
			}
			O() << "TOTAL" << "\t" << sum << "\n";
		}
	};
}

char CountStaticInstructions::ID = 0;
static RegisterPass<CountStaticInstructions> X("count_static", "CountStaticInstructions Pass");
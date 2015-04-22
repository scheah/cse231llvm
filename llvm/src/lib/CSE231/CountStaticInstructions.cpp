//===- CountStaticInstructions.cpp - 				    ---------------===//
//
// Write a pass that counts the number of instructions in the program. 
// The pass should output the total number of instructions, as well as a per-instruction count. 
// For example, if there are 2 loads and 3 adds, then the output should be similar to this:
// 		load  2
// 		add   3
// 		TOTAL 5
//===----------------------------------------------------------------------===//

//#define DEBUG_TYPE "CountStaticInstructions"
//#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

//#include "llvm/IR/InstIterator.h" //unavailable
#include "llvm/IR/Module.h"
#include <map>
#include <stdio.h>

using namespace llvm;
using namespace std;

//Do not use the STATISTIC convenience macro provided by LLVM.
//STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
  // CountStaticInstructions - The first implementation, without getAnalysisUsage.
	struct CountStaticInstructions : public ModulePass {
		static char ID; // Pass identification, replacement for typeid
		static map<const char*, int> instCounter;
		
		CountStaticInstructions() : ModulePass(ID) {}

		/*virtual bool runOnFunction(Function &F) {
			for (Function::iterator B = F->begin(); B != F->end(); ++B) { 
				for (BasicBlock::iterator I = B->begin(); I != B->end(); ++I) { 
				
				}
			}
		}*/
		virtual bool runOnModule(Module &M) {
			for (Module::iterator F = M.begin(); F != M.end(); ++F) { 
				for (Function::iterator B = F->begin(); B != F->end(); ++B) { 
					for (BasicBlock::iterator I = B->begin(); I != B->end(); ++I) { 
						instCounter[I->getOpcodeName()]++;
					}
				}
				/*for (inst_iterator I = inst_begin(F), I != inst_end(F); ++I) {
					instCounter[I->getOpcodeName()]++;
				}*/
			}
			return false; // not modifying the module
		}
		virtual void print(raw_ostream&	O, const Module* M) const {
			int sum = 0;
			for(map<const char*, int>::iterator iterator = instCounter.begin(); iterator != instCounter.end(); ++iterator) {
				printf("%s\t%d\n", iterator->first, iterator->second);
				sum += iterator->second;
			}
			printf("%s\t%d\n", "TOTAL", sum);
		}
	};
}

char CountStaticInstructions::ID = 0;
static RegisterPass<CountStaticInstructions> X("count_static", "CountStaticInstructions Pass");
#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include <stdio.h>
#include "irgen.h"
#include "llvm/ExecutionEngine//GenericValue.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

//--------------------------------------------------------------------
// It's really of incredibly shit
//--------------------------------------------------------------------
class LLMDInterpreter {
private:
    IRGenerator irgen;

public:
    LLMDInterpreter(const char *mdFileName) : irgen(mdFileName) {}

    void execute() {
        // Official example doesn't not work if we follow that
        // Note that we *must* set datalayout before passing the module to
        // engine builder
        std::unique_ptr<TargetMachine> targetMachine(
            EngineBuilder().selectTarget());
        irgen.theModule.setDataLayout(targetMachine->createDataLayout());

        ExecutionEngine *EE =
            EngineBuilder(std::unique_ptr<Module>(&irgen.theModule)).create();

        std::vector<GenericValue> noargs;
        EE->runFunction(irgen.func, noargs);

        // Note that we must flush stream or we can not get output result
        outs().flush();
    }
};

#endif  // !_INTERPRETER_H

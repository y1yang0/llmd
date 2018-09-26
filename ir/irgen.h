#ifndef _IRGEN_H
#define _IRGEN_H

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

#include "callbacks.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalValue.h"

using namespace llvm;

class LLVMIRGenerator {
public:
    LLVMIRGenerator();
    bool parseMarkdown(const char *fileName);
    inline void dump() { theModule.dump(); }
    inline Function *getFunction() { return func; }

private:
    Function *createFunction();
    void emitIfImpl(Value *cond, const std::string &label);

public:
    void emitVariable(const std::string &varName, int value);
    void emitBinaryExpr(const std::string &expr);
    void emitLabel(const std::string &label);
    void emitIf(const std::string &name, const std::string &label);
    void emitIf(int constVal, const std::string &label);
    void emitPrint(const std::string & text);

private:
    LLVMContext context;
    IRBuilder<> builder;
    Module theModule;
    std::map<std::string, AllocaInst *> values;
    Function *func;
};

#endif  // !_IRGEN_H

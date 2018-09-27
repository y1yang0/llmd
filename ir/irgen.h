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
#include "util.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

class LLVMIRGenerator {
public:
    LLVMIRGenerator(const char *mdFileName);
    inline void dumpToConsole() { theModule.dump(); }
    void dumpToFile();

private:
    bool parseMarkdown(const char *fileName);
    void emitIfImpl(Value *cond, const std::string &label);

public:
    void emiFunction(const std::string &funcName);
    void emitVariable(const std::string &varName, int value);
    void emitBinaryExpr(const std::string &varName, const std::string &expr);
    void emitLabel(const std::string &label);
    void emitIf(const std::string &name, const std::string &label);
    void emitIf(int constVal, const std::string &label);
    void emitPrint(const std::string &text);
    void emitReturn();

private:
    LLVMContext context;
    IRBuilder<> builder;
    Module theModule;
    std::map<std::string, AllocaInst *> localValues;
    Function *func;
    std::string mdFileName;
};

#endif  // !_IRGEN_H

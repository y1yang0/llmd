#include <exception>
#include "irgen.h"

//--------------------------------------------------------------------
// WTF we can only include these headers on cpp rather than irgen.h?
//--------------------------------------------------------------------
#include "../parser/buffer.h"
#include "../parser/markdown.h"

using namespace llvm;

LLVMIRGenerator::LLVMIRGenerator(const char* mdFileName)
    : context(),
      builder(context),
      theModule("llmd", context),
      localValues(),
      func(nullptr) {
    parseMarkdown(mdFileName);
}

void LLVMIRGenerator::dumpToFile() {
    std::string fileName = func->getName().operator std::string() + ".ll";
    raw_fd_ostream* fout = new raw_fd_ostream(
        StringRef(fileName), std::error_code(), sys::fs::OpenFlags());
    theModule.print(*fout, nullptr);
    fout->flush();
    delete fout;
}

bool LLVMIRGenerator::parseMarkdown(const char* fileName) {
    constexpr int READ_UNIT = 1024;
    constexpr int OUTPUT_UNIT = 64;

    struct buf *ib, *ob;
    int ret;
    FILE* in = stdin;

    struct sd_callbacks callbacks;
    memset(&callbacks, 0, sizeof(sd_callbacks));
    registerInterestingCallbacks(callbacks);

    struct sd_markdown* markdown;

    in = fopen(fileName, "r");
    if (!in) {
        fprintf(stderr, "Unable to open input file \"%s\": %s\n", fileName,
                strerror(errno));
        return false;
    }
    ib = bufnew(READ_UNIT);
    bufgrow(ib, READ_UNIT);
    while ((ret = fread(ib->data + ib->size, 1, ib->asize - ib->size, in)) >
           0) {
        ib->size += ret;
        bufgrow(ib, ib->size + READ_UNIT);
    }

    if (in != stdin) fclose(in);

    ob = bufnew(OUTPUT_UNIT);

    markdown = sd_markdown_new(0, 16, &callbacks, this);

    sd_markdown_render(ob, ib->data, ib->size, markdown);
    sd_markdown_free(markdown);

    ret = fwrite(ob->data, 1, ob->size, stdout);
    bufrelease(ib);
    bufrelease(ob);

    return (ret < 0) ? false : true;
}

void LLVMIRGenerator::emitVariable(const std::string& varName, int value) {
    Value* val = ConstantInt::get(Type::getInt32Ty(context), value);

    IRBuilder<> tempBuilder(&func->getEntryBlock(),
                            func->getEntryBlock().begin());
    AllocaInst* localVar =
        tempBuilder.CreateAlloca(Type::getInt32Ty(context), nullptr, varName);

    builder.CreateStore(val, localVar);
    localValues[varName] = localVar;
}

void LLVMIRGenerator::emitBinaryExpr(const std::string& varName,
                                     const std::string& expr) {
    std::string lhs, rhs;
    char op;
    for (int i = 1, sec = 0; expr[i] != '`'; i++) {
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*') {
            op = expr[i];
            sec = 1;
            continue;
        }
        if (!sec) {
            lhs += expr[i];
        } else {
            rhs += expr[i];
        }
    }

    // [var](`lhs`)
    if (lhs.length() != 0 && rhs.length() == 0) {
        Value* lhsVal = builder.CreateLoad(localValues[lhs]);
        builder.CreateStore(lhsVal, localValues[varName]);
        return;
    }

    // [var](`lhs+rhs`)
    if (lhs.length() != 0 && rhs.length() != 0) {
        Value *lhsVal, *rhsVal;
        // if it's a literal constant
        if (lhs[0] >= '0' && lhs[0] <= '9') {
            lhsVal =
                ConstantInt::get(Type::getInt32Ty(context), atoi(lhs.c_str()));
        } else {
            lhsVal = localValues[lhs];
            lhsVal = builder.CreateLoad(lhsVal, lhs);
        }

        if (rhs[0] >= '0' && rhs[0] <= '9') {
            rhsVal =
                ConstantInt::get(Type::getInt32Ty(context), atoi(rhs.c_str()));
        } else {
            rhsVal = localValues[rhs];
            rhsVal = builder.CreateLoad(rhsVal, rhs);
        }

        switch (op) {
            case '+': {
                Value* calcTmp = builder.CreateAdd(lhsVal, rhsVal);
                builder.CreateStore(calcTmp, localValues[varName]);
                break;
            }
            case '-': {
                Value* calcTmp = builder.CreateSub(lhsVal, rhsVal);
                builder.CreateStore(calcTmp, localValues[varName]);
                break;
            }
            case '*': {
                Value* calcTmp = builder.CreateMul(lhsVal, rhsVal);
                builder.CreateStore(calcTmp, localValues[varName]);
                break;
            }
            default:
                break;
        }

        return;
    }
}

void LLVMIRGenerator::emitLabel(const std::string& label) {
    BasicBlock* labelBlock = BasicBlock::Create(context, label, func);
    builder.CreateBr(labelBlock);
    builder.SetInsertPoint(labelBlock);
}

void LLVMIRGenerator::emitPrint(const std::string& text) {
    // Declare i32 @printf(i8*, ...)
    auto printFunc = theModule.getOrInsertFunction(
        "printf",
        FunctionType::get(IntegerType::getInt32Ty(context),
                          PointerType::get(Type::getInt8Ty(context), 0), true));
    // Construct argument list for printf
    auto res = parseInterpolateString(text);

    std::vector<Value*> argsV;

    Value* formatValue = builder.CreateGlobalStringPtr(res[0]);
    argsV.push_back(formatValue);
    for (int i = 1; i < res.size(); i++) {
        std::string& varName = res[i];
        AllocaInst* localStrVar =
            builder.CreateAlloca(Type::getInt32Ty(context), nullptr, varName);
        Value* t = builder.CreateLoad(localValues[varName]);
        builder.CreateStore(t, localStrVar);
        argsV.push_back(builder.CreateLoad(localStrVar));
    }

    // Create function call
    builder.CreateCall(printFunc, argsV, "c_printf");
}

void LLVMIRGenerator::emitReturn() { builder.CreateRetVoid(); }

void LLVMIRGenerator::emitIf(int constVal, const std::string& label) {
    Value* cond = builder.CreateICmpNE(
        ConstantInt::get(Type::getInt32Ty(context), constVal),
        ConstantInt::get(Type::getInt32Ty(context), 0), "ifcond_const");
    emitIfImpl(cond, label);
}

void LLVMIRGenerator::emitIf(const std::string& name,
                             const std::string& label) {
    Value* v = builder.CreateLoad(localValues[name], name);
    Value* cond = builder.CreateICmpNE(
        v, ConstantInt::get(Type::getInt32Ty(context), 0), "ifcond_var");
    emitIfImpl(cond, label);
}

void LLVMIRGenerator::emitIfImpl(Value* cond, const std::string& label) {
    BasicBlock* thenBlock;
    bool findLabel = false;
    for (auto& bb : *func) {
        if (bb.getName() == label) {
            findLabel = true;
            thenBlock = &bb;
        }
    }
    if (!findLabel) {
        throw new std::exception("specific label does not exist");
    }
    BasicBlock* endBlock = BasicBlock::Create(context, "end", func);
    builder.CreateCondBr(cond, thenBlock, endBlock);
    builder.SetInsertPoint(endBlock);
}

void LLVMIRGenerator::emiFunction(const std::string& funcName) {
    func = Function::Create(FunctionType::get(builder.getVoidTy(), false),
                            Function::ExternalLinkage, funcName, &theModule);
    func->setCallingConv(llvm::CallingConv::C);
    BasicBlock* entry = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entry);
}
#include <exception>
#include "irgen.h"
//--------------------------------------------------------------------
// WTF we can only include these headers on cpp rather than irgen.h?
//--------------------------------------------------------------------
#include "../parser/buffer.h"
#include "../parser/markdown.h"

using namespace llvm;

LLVMIRGenerator::LLVMIRGenerator()
    : context(), builder(context), theModule("llmd", context), values() {
    // To explicitly create a new func
    func = createTopFunction();
}

bool LLVMIRGenerator::parseMarkdown(const char* fileName) {
    constexpr int READ_UNIT = 1024;
    constexpr int OUTPUT_UNIT = 64;

    struct buf *ib, *ob;
    int ret;
    FILE* in = stdin;

    struct sd_callbacks callbacks;
    memset(&callbacks, 0, sizeof(sd_callbacks));
    registerCallbacks(callbacks);

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
    values[varName] = localVar;
}

void LLVMIRGenerator::emitBinaryExpr(const std::string& expr) {
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

    if (lhs.length() != 0 && rhs.length() != 0) {
        Value *lhsVal, *rhsVal;
        // if it's a literal constant
        if (lhs[0] >= '0' && lhs[0] <= '9') {
            lhsVal =
                ConstantInt::get(Type::getInt32Ty(context), atoi(lhs.c_str()));
        } else {
            lhsVal = values[lhs];
            lhsVal = builder.CreateLoad(lhsVal, lhs);
        }

        if (rhs[0] >= '0' && rhs[0] <= '9') {
            rhsVal =
                ConstantInt::get(Type::getInt32Ty(context), atoi(rhs.c_str()));
        } else {
            rhsVal = values[rhs];
            rhsVal = builder.CreateLoad(rhsVal, rhs);
        }

        switch (op) {
            case '+':
                builder.CreateAdd(lhsVal, rhsVal, "addtmp");
                break;
            case '-':
                builder.CreateSub(lhsVal, rhsVal, "subtmp");
                break;
            case '*':
                builder.CreateMul(lhsVal, rhsVal, "multmp");
            default:
                break;
        }
    }
}

void LLVMIRGenerator::emitLabel(const std::string& label) {
    BasicBlock* labelBlock = BasicBlock::Create(context, label, func);
    builder.SetInsertPoint(labelBlock);
}

void LLVMIRGenerator::emitIf(int constVal, const std::string& label) {
    Value* cond = builder.CreateICmpNE(
        ConstantInt::get(Type::getInt32Ty(context), constVal),
        ConstantInt::get(Type::getInt32Ty(context), 0), "ifcond_const");
    emitIfImpl(cond, label);
}

void LLVMIRGenerator::emitIf(const std::string& name,
                             const std::string& label) {
    Value* v = builder.CreateLoad(values[name], name);
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

Function* LLVMIRGenerator::createTopFunction() {
    // void main(){}
    Function* func =
        Function::Create(FunctionType::get(builder.getVoidTy(), false),
                         Function::ExternalLinkage, "main", &theModule);
    BasicBlock* entry = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entry);
    return func;
}
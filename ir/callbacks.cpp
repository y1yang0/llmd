#include "irgen.h"
#include "../parser/markdown.h"
#include <iostream>
#include <string>

static std::string buf2string(const buf*const b) {
    if (b == nullptr) {
        return "";
    }
    std::string str;
    for (int i = 0; i < b->size; i++) {
        str += b->data[i];
    }
    return str;
}

void registerCallbacks(sd_callbacks & cbs) {
    cbs.link = link;
}

int link(buf * ob, const buf * link, const buf * title, const buf * content, void * opaque) {
    LLVMIRGenerator* gen = (LLVMIRGenerator*)opaque;

    std::string stringValue = buf2string(link);
    std::string name = buf2string(content);
    // it's a literal number
    if (stringValue.length() == 0 || stringValue[0] != '`') {
        gen->emitVariable(
            name,
            stringValue.length() == 0 ? 0 : atoi(stringValue.c_str()));

    }
    else {  // otherwise, it's a expression
        gen->emitBinaryExpr(stringValue);
    }
    return 0;
}
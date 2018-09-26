#include <iostream>
#include <string>
#include "../parser/markdown.h"
#include "irgen.h"

static std::string buf2string(const buf* const b) {
    if (b == nullptr) {
        return "";
    }
    std::string str;
    for (int i = 0; i < b->size; i++) {
        str += b->data[i];
    }
    return str;
}

void registerCallbacks(sd_callbacks& cbs) {
    cbs.link = link;
    cbs.image = image;
    cbs.blockquote = blockquote;
    cbs.emphasis = emphasis;
}

int emphasis(buf* ob, const buf* text, void* opaque) {
    LLVMIRGenerator* gen = (LLVMIRGenerator*)opaque;

    gen->emitLabel(buf2string(text));
    return 0;
}

int image(buf* ob, const buf* link, const buf* title, const buf* alt,
          void* opaque) {
    LLVMIRGenerator* gen = (LLVMIRGenerator*)opaque;
    std::string stringValue = buf2string(alt);
    std::string label = buf2string(link);

    if (label.length() == 0) {
        throw new std::exception("jump label can no leave empty");
    }

    // if condition is empty
    if (stringValue.length() == 0) {
        return 1;
    }

    if (stringValue[0] == '`') {
        gen->emitIf(stringValue.substr(1, stringValue.find_last_of('`') - 1),
                    label);
    } else {
        gen->emitIf(atoi(stringValue.c_str()), label);
    }

    return 0;
}

int link(buf* ob, const buf* link, const buf* title, const buf* content,
         void* opaque) {
    LLVMIRGenerator* gen = (LLVMIRGenerator*)opaque;

    std::string stringValue = buf2string(link);
    std::string name = buf2string(content);
    // it's a literal number
    if (stringValue.length() == 0 || stringValue[0] != '`') {
        gen->emitVariable(
            name, stringValue.length() == 0 ? 0 : atoi(stringValue.c_str()));

    } else {  // otherwise, it's a expression
        gen->emitBinaryExpr(stringValue);
    }
    return 0;
}

void blockquote(buf* ob, const buf* text, void* opaque) {
    // std::cout << buf2string(text);
}

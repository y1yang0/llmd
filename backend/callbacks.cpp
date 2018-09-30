#include <cstring>
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

void doc_footer(buf* ob, void* opaque) {
    IRGenerator* gen = (IRGenerator*)opaque;
    gen->emitReturn();
}

void registerInterestingCallbacks(sd_callbacks& cbs) {
    cbs.link = link;
    cbs.image = image;
    cbs.blockquote = blockquote;
    cbs.emphasis = emphasis;
    cbs.header = header;
    cbs.doc_footer = doc_footer;
}

int emphasis(buf* ob, const buf* text, void* opaque) {
    IRGenerator* gen = (IRGenerator*)opaque;

    gen->emitLabel(buf2string(text));
    return CB_SUCCESS;
}

int image(buf* ob, const buf* link, const buf* title, const buf* alt,
          void* opaque) {
    IRGenerator* gen = (IRGenerator*)opaque;
    std::string stringValue = buf2string(alt);
    std::string label = buf2string(link);

    if (label.length() == 0) {
        throw new std::exception("jump label can no leave empty");
    }

    if (stringValue.length() == 0) {
        return CB_FAILED;
    }

    if (stringValue[0] == '`') {
        gen->emitIf(stringValue.substr(1, stringValue.find_last_of('`') - 1),
                    label);
    } else {
        gen->emitIf(atoi(stringValue.c_str()), label);
    }

    return CB_SUCCESS;
}

int link(buf* ob, const buf* link, const buf* title, const buf* content,
         void* opaque) {
    IRGenerator* gen = (IRGenerator*)opaque;

    std::string stringValue = buf2string(link);
    std::string name = buf2string(content);
    // it's a literal number
    if (stringValue.length() == 0 || stringValue[0] != '`') {
        gen->emitVariable(
            name, stringValue.length() == 0 ? 0 : atoi(stringValue.c_str()));

    } else {  // otherwise, it's an unevaled expression
        gen->emitBinaryExpr(name,stringValue);
    }
    return CB_SUCCESS;
}

void blockquote(buf* ob, const buf* text, void* opaque) {
    IRGenerator* gen = (IRGenerator*)opaque;
    std::string t = buf2string(text);
    gen->emitPrint(t);
}

void header(buf* ob, const buf* text, int level, void* opaque) {
    IRGenerator* gen = (IRGenerator*)opaque;

    std::string funcName = buf2string(text);

    // Check if it's a valid function name
    if (funcName.length() == 0 || !isalpha(funcName[0])) {
        return;
    }
    for (int i = 1; i < funcName.length(); i++) {
        if (!isalnum(funcName[i])) {
            return;
        }
    }
    // Create a function by give name
    gen->emiFunction(funcName);
}

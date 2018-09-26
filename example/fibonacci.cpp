#include "../ir/irgen.h"
#include <cstdlib>

int main() {
    LLVMIRGenerator gen;
    gen.parseMarkdown("fibonacci.md");
    gen.dump();
    system("pause");
    return 0;
}
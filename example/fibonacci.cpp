#include <cstdlib>
#include "../ir/irgen.h"

int main() {
    LLVMIRGenerator gen("fibonacci.md");
    gen.dumpToConsole();
    system("pause");
    return 0;
}
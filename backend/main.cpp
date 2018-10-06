// llvm based markdown programming language
#include <cstdlib>
#include "../backend/interpreter.h"

int main(int argc, char* argv[]) {
    InitializeNativeTarget();

    if (argc != 2) {
        std::cerr << "Specify your markdown source file!\n";
        return 1;
    }
    LLMDInterpreter interp(argv[1]);
    interp.execute();
    return 0;
}
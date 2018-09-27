#include <cstdlib>
#include "../ir/irgen.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Specify your markdown source file!\n";
        return 1;
    }
    LLVMIRGenerator gen(argv[1]);
    gen.dumpToFile();
    std::cout << argv[1] << ".ll generated.\n";
    return 0;
}
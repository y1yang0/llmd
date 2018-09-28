CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(llmd)     

file(COPY ${CMAKE_SOURCE_DIR}/example/fibonacci.md ${CMAKE_SOURCE_DIR}/example/helloworld.md 
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories("F:\\cpplib\\llvm-5.0.0.src\\build\\include" "F:\\cpplib\\llvm-5.0.0.src\\include")
aux_source_directory(./ir src) 
aux_source_directory(./parser src)
link_directories("F:\\cpplib\\llvm-5.0.0.src\\build\\Debug\\lib")
add_executable(llmd ${src})

target_link_libraries(llmd LLVMCore.lib LLVMSupport.lib LLVMBinaryFormat.lib)
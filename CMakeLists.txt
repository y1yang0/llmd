CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(llmd)     

file(COPY ${CMAKE_SOURCE_DIR}/example/fibonacci.md ${CMAKE_SOURCE_DIR}/example/helloworld.md 
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories("F:\\cpplib\\llvm-5.0.0.src\\build\\include" "F:\\cpplib\\llvm-5.0.0.src\\include")
aux_source_directory(./backend src) 
aux_source_directory(./parser src)
link_directories("F:\\cpplib\\llvm-5.0.0.src\\build\\Debug\\lib")
add_executable(llmd ${src})

target_link_libraries(llmd
LLVMInterpreter.lib
LLVMCoroutines.lib
LLVMLibDriver.lib
LLVMObjectYAML.lib
LLVMCoverage.lib
LLVMDlltoolDriver.lib
LLVMOption.lib
LLVMXCoreDisassembler.lib
LLVMXCoreCodeGen.lib
LLVMXCoreDesc.lib
LLVMXCoreInfo.lib
LLVMXCoreAsmPrinter.lib
LLVMSystemZDisassembler.lib
LLVMSystemZCodeGen.lib
LLVMSystemZAsmParser.lib
LLVMSystemZDesc.lib
LLVMSystemZInfo.lib
LLVMSystemZAsmPrinter.lib
LLVMOrcJIT.lib
LLVMMIRParser.lib
LLVMLTO.lib
LLVMPasses.lib
LLVMObjCARCOpts.lib
LLVMipo.lib
LLVMInstrumentation.lib
LLVMVectorize.lib
LLVMLinker.lib
LLVMIRReader.lib
LLVMAsmParser.lib
LLVMSymbolize.lib
LLVMDebugInfoPDB.lib
LLVMDebugInfoDWARF.lib
LLVMLineEditor.lib
LLVMTableGen.lib
LLVMX86Disassembler.lib
LLVMX86AsmParser.lib
LLVMX86CodeGen.lib
LLVMGlobalISel.lib
LLVMSelectionDAG.lib
LLVMAsmPrinter.lib
LLVMDebugInfoCodeView.lib
LLVMDebugInfoMSF.lib
LLVMCodeGen.lib
LLVMScalarOpts.lib
LLVMInstCombine.lib
LLVMTransformUtils.lib
LLVMBitWriter.lib
LLVMX86Desc.lib
LLVMMCDisassembler.lib
LLVMX86Info.lib
LLVMX86AsmPrinter.lib
LLVMX86Utils.lib
LLVMMCJIT.lib
LLVMExecutionEngine.lib
LLVMTarget.lib
LLVMAnalysis.lib
LLVMProfileData.lib
LLVMRuntimeDyld.lib
LLVMObject.lib
LLVMMCParser.lib
LLVMBitReader.lib
LLVMMC.lib
LLVMCore.lib
LLVMBinaryFormat.lib
LLVMSupport.lib
LLVMDemangle.lib)
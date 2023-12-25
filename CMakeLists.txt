cmake_minimum_required(VERSION 3.16)

project(S-Lang VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic")

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add the LLVM include directories and compile definitions
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Function to set library output directory
function(set_lib_output_directory target)
    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
        LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
    )
endfunction()

# Lexer library
add_library(Lexer ${PROJECT_SOURCE_DIR}/src/lexer.cpp)
target_include_directories(Lexer PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_lib_output_directory(Lexer)

# AST library
add_library(AST ${PROJECT_SOURCE_DIR}/src/ast.cpp)
target_include_directories(AST PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(AST PUBLIC Lexer)
set_lib_output_directory(AST)

# Parser library
add_library(Parser ${PROJECT_SOURCE_DIR}/src/parser.cpp)
target_include_directories(Parser PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(Parser PUBLIC Lexer AST)
set_lib_output_directory(Parser)

# Link against LLVM libraries and project libraries
llvm_map_components_to_libnames(LLVM_LIBS core)

# CodeGen library
add_library(CodeGen ${PROJECT_SOURCE_DIR}/src/codegen.cpp)
target_include_directories(CodeGen PUBLIC "${PROJECT_SOURCE_DIR}/include" "${LLVM_INCLUDE_DIRS}")
target_link_libraries(CodeGen PUBLIC Parser Lexer AST ${LLVM_LIBS})
set_lib_output_directory(CodeGen)

# SlangProgram Library
add_library(SlangProgram ${PROJECT_SOURCE_DIR}/src/slang.cpp)
target_include_directories(SlangProgram PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(SlangProgram PUBLIC Lexer AST Parser CodeGen)
set_lib_output_directory(SlangProgram)

# Add the executable in app/main.cpp
add_executable(slang ${PROJECT_SOURCE_DIR}/app/main.cpp)
target_include_directories(slang PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(slang PUBLIC LLVMSupport Lexer AST Parser CodeGen SlangProgram)

# For testing the Lexer and Parser
enable_testing()
add_subdirectory(tests)

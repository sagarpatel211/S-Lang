cmake_minimum_required(VERSION 3.17)

project(slang VERSION 0.0.0)
add_compile_options()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Find LLVM, Flex and Bison
find_package(LLVM REQUIRED CONFIG)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Add the LLVM include directories and compile definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Collect all source and header files
file(GLOB_RECURSE SRC_FILES "app/main.cpp")
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "include/*.hpp")

flex_target(Scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
bison_target(Parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
add_flex_bison_dependency(Scanner Parser)

# Add your compiler's source and header files
add_executable(slg
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs all)
target_link_libraries(slg ${llvm_libs})

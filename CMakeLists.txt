cmake_minimum_required(VERSION 3.17)

project(SLang VERSION 1.0.0)
add_compile_options()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

# Add the LLVM include directories and compile definitions
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Collect all source and header files
file(GLOB_RECURSE MAIN_FILE "app/main.cpp")
file(GLOB_RECURSE INCLUDE_FILES "include/*.hpp")
message(STATUS "Found ${INCLUDE_FILES}")
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
message(STATUS "Found ${SRC_FILES}")

# Add compiler's source files
add_executable(slang ${MAIN_FILE} ${INCLUDE_FILES} ${SRC_FILES})

# Add header files
target_include_directories(slang PRIVATE "${PROJECT_SOURCE_DIR}/include")

# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs all)
target_link_libraries(slang ${llvm_libs})

# Set output directory of binary files
set_target_properties(slang PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

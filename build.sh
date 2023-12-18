#!/bin/bash

# Clone the S-Lang repository
git clone https://github.com/sagarpatel211/S-Lang.git

# Enter the cloned project directory
cd S-Lang

# Create a 'build' directory to hold the build system files
mkdir -p build

# Update package lists
sudo apt-get update

# Install necessary dependencies
sudo apt-get install -y cmake
sudo apt-get install -y clang
sudo apt-get install -y gcc
sudo apt-get install -y g++
sudo apt-get install -y build-essential
sudo apt-get install -y libboost-system-dev
sudo apt-get install -y libboost-thread-dev
sudo apt-get install -y libboost-program-options-dev
sudo apt-get install -y libboost-test-dev

# Remove existing version of python3-lldb-14 if installed
sudo apt-get purge python3-lldb-14

# Download and install LLVM version 17
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 17

# Configure the project with CMake specifying a build type
# (Replace 'Release' with your desired build type if different)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Compile the project
cmake --build build --config Release

# Change to the build directory and run tests
cd build
ctest -C Release

# List all binaries in the bin directory
ls bin
# Using Ubuntu 20.04 as the base image
FROM ubuntu:20.04

# Set the DEBIAN_FRONTEND environment variable to noninteractive to disable prompts
ARG DEBIAN_FRONTEND=noninteractive

# Set the working directory
WORKDIR /usr/src/app

# Print a message to the console
RUN echo "Starting build process..."

# Install CMake, a C++ compiler, LLVM, and required tools
RUN apt-get update && \
    apt-get install -y \
    wget \
    git \
    cmake \
    clang \
    gcc \
    g++ \
    build-essential \
    libboost-system-dev \
    libboost-thread-dev \
    libboost-program-options-dev \
    libboost-test-dev \
    lsb-release \
    software-properties-common \
    gnupg && \
    apt-get purge python3-lldb-14 -y && \
    wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 17 && \
    rm -rf /var/lib/apt/lists/*

# Copy the source files into the working directory
COPY CMakeLists.txt ./
COPY src/ ./src/
COPY include/ ./include/
COPY app/ ./app/
COPY tests/ ./tests/

# Run CMake to generate the buildsystem
RUN cmake -S . -B build

# Build the project
RUN cmake --build build

# Run the tests
RUN ./build/bin/test_lexer
RUN ./build/bin/test_parser

# Add the build/bin directory to the PATH
ENV PATH="/usr/src/app/build/bin:${PATH}"

# Provide instructions for using the compiler
RUN echo "S-Lang build complete!"
RUN echo "Use `./slang [options] [file]` to use the compiler"

# Slanguage
<!-- BADGES -->
[![CMake](https://github.com/sagarpatel211/S-Lang/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/sagarpatel211/S-Lang/actions/workflows/cmake.yml)


<!-- DESCRIPTION -->
A Programming Language for GenZ! I got into wanting to design my own programming language after watching a quick [video](https://www.youtube.com/watch?v=BT2Cv-Tjq7Q) about [LLVM](https://llvm.org/) from [Fireship](https://www.youtube.com/@Fireship). I then began with learning to use LLVM through their [Kaleidoscope tutorial](https://llvm.org/docs/tutorial/) and then started from scratch :)! Check out the features listed near the end!

> I know I know, the language is not as complete as ones used in real-life, but it was just a project to learn more about the frontend of compilers. If you'd like to add features to the language, please make a pull request!

<!-- TABLE OF CONTENTS -->
## Table of Contents
- [To Do](#to-do)
- [Dependencies](#dependencies)
- [Project Structure](#project-structure)
- [Installation & Usage](#installation--usage)
- [Features](#features)
- [Examples](#examples)
- [Contributors](#contributors)


<!-- Dependencies AND TOOLS -->
## Dependencies
- C++ Compiler (C++20 or later)
- CMake (3.16 or later)
- LLVM (14 or later)

## To Do
- Fix documentation of Parser.hpp
- Add documentation of Codegen.hpp
- Refactor Codegen.cpp (switch to snakecase, be consistent even though their API uses camelCase)
- Add documentation of Parser.cpp
- Add documentation of Codegen.cpp
- Complete test_parser
- Test if nested control flow statements work as well as multiple ong? (else-if) work
- Finish examples and their README and compile steps
- Finish this README
- Test it works on Windows and provide instructions in here
- Add a readthedocs page
- Make sure char and string concatenation work
- Double check global vars work
- Potentially add the @throws in the documentation for the parser, lexer, and codegen
- Test the examples work
- Add the Grammar of my code somewhere


<!-- PROJECT STRUCTURE -->
## Project Structure
  ```
  ├── README.md
  ├── LICENSE
  ├── Dockerfile
  ├── CONTRIBUTORS
  ├── CMakeLists.txt
  ├── .gitignore
  ├── .gitattributes
  ├── .clang-format
  ├── app
  │   └── main.cpp
  ├── include
  │   ├── ast.hpp
  │   ├── codegen.hpp
  │   ├── debug_stream.hpp
  │   ├── exceptions.hpp
  │   ├── lexer.hpp
  │   ├── parser.hpp
  │   └── slang.hpp
  ├── src
  │   ├── ast.cpp
  │   ├── codegen.cpp
  │   ├── lexer.cpp
  │   ├── parser.cpp
  │   └── slang.cpp
  ├── tests
  │   ├── CMakeLists.txt
  │   ├── test_lexer.cpp
  │   └── test_parser.cpp
  ├── examples
  |   ├── C++ Demos
  |   │   ├── README.md
  |   │   ├── math
  |   │   │   ├── main.cpp
  |   │   │   ├── math.slg
  |   │   │   └── compile.sh
  |   │   └── printing
  |   │       ├── main.cpp
  |   │       ├── printing.slg
  |   │       └── compile.sh
  │   ├── 1. helloworld.slg
  │   ├── 2. simple_chars.slg
  │   ├── 3. simple_nums.slg
  │   ├── 4. simple_bools.slg
  │   ├── 5. simple_recursion.slg
  │   ├── 6. conditionals.slg
  │   ├── 7. loops.slg
  │   └── README.md
  └── .github
      └── workflows
          └── cmake.yml
  ```


<!-- INSTALLATION & USAGE -->
## Installation & Usage

### Linux:
> Should work on most distos where clang/llvm is supported but I've only tested on Debian-based ones!
> For Ubuntu users, to do the following in a single command, run the `build.sh` script which also includes installation of project dependencies (eg. CMake, LLVM, etc.)!

<!-- DECIDE FOR build.sh if I require them to clone directory or if it does it for them?

Provide a script for MacOS as well -->

```bash
# Assuming you've installed all the dependancies
$ git clone https://github.com/sagarpatel211/S-Lang.git   # Clone the Repository
$ cd S-Lang   # Enter project directory
$ mkdir build   # Create `build` directory to hold the buildsystem files
$ cd build   # Enter build/ directory
$ cmake ..   # Use the CMake build tool to generate a buildsystem
$ cmake --build .   # Build the program and test executables
$ cd bin   # Enter directory where executables are located
```

### MacOS
> In Progress

### Windows:
> In Progress (bruh, why is Windows so difficult)


<!-- FEATURES -->
## Features
<!--
True: 
False:

- TO DO
- INCLUDE HERE THE KEYWORDS IN RESPECT TO A DICTIONARY THAT EXPLAINS THEM OR Knowyourmeme
-->

<!-- EXAMPLES -->
## Examples
- Take a look at the `examples/` directory to get started with using S-Lang!


<!-- CONTRIBUTORS -->
## Contributors

| <a href="https://github.com/sagarpatel211" target="_blank">**Sagar Patel**</a> |
| :---: |
| [![Sagar Patel](https://avatars1.githubusercontent.com/u/34544263?s=200)](https://github.com/sagarpatel211)    |
| <a href="https://github.com/sagarpatel211" target="_blank">github.com/sagarpatel211</a> |
| [Email](mailto:sa24pate@uwaterloo.ca) |
| [Website](https://sagarpatel211.github.io/) |

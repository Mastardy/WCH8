cmake_minimum_required(VERSION 3.26)
project(WCH8 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Disassembler.cpp
        src/Disassembler.hpp
        src/Assembler.cpp
        src/Assembler.hpp
        src/Emulator.cpp
        src/Emulator.hpp
        src/Utility.hpp
        src/Utility.cpp)

cmake_minimum_required(VERSION 3.24)
set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
set(CMAKE_RC_COMPILER "llvm-rc")

project(Yadro)

set(CMAKE_CXX_STANDARD 17)
set(PCH src/pch.h)
set(SOURCE_FILES
        src/main.cpp
        src/Tape.hpp)

add_executable(Yadro ${SOURCE_FILES})
target_precompile_headers(Yadro
        PRIVATE ${PCH})

include(Properties.cmake)

cmake_minimum_required(VERSION 3.24)

set(Boost_NO_WARN_NEW_VERSIONS 1)
set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
set(CMAKE_RC_COMPILER "llvm-rc")

project(Yadro)
find_package(Boost 1.81 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(PCH src/pch.h)
set(SOURCE_FILES
        src/main.cpp)

add_executable(Yadro ${SOURCE_FILES})
target_precompile_headers(Yadro
        PRIVATE ${PCH})
target_link_libraries(Yadro ${Boost_LIBRARIES})

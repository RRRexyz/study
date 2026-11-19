cmake_minimum_required(VERSION 3.10.0)
project(study VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-unknown-warning-option)
endif()

add_executable(main main.cpp)

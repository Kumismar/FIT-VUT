# Author: Ondřej Koumar
# Date: 2024-03-18

cmake_minimum_required(VERSION 3.20)
project(Parser LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
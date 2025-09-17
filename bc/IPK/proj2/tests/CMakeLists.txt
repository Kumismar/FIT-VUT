cmake_minimum_required(VERSION 3.14)
project(ipk_project_2)

set(PROJECTDIR
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(OBJS
    ${PROJECTDIR}/build/argument_processing.o
    ${PROJECTDIR}/build/error_functions.o
    ${PROJECTDIR}/build/message_parsing.o
    ${PROJECTDIR}/build/pointerList.o
    ${PROJECTDIR}/build/stack.o
)

enable_testing()

add_executable(
  testos
  lexical_analysis_tests.cpp
  syntax_analysis_tests.cpp
  stack_tests.cpp
)
target_link_libraries(
  testos
  ${OBJS}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testos)

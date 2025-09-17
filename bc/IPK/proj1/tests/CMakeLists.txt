cmake_minimum_required(VERSION 3.14)
project(ipk_project_1)

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
    ${PROJECTDIR}/build/list.o
)

enable_testing()

add_executable(
  testos
  cmd_args_tests.cpp
  list_tests.cpp
)
target_link_libraries(
  testos
  ${OBJS}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testos)
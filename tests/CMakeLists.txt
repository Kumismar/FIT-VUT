cmake_minimum_required(VERSION 3.14)
project(isa_project)

set(PROJECTDIR
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

file(GLOB OBJS
    ${PROJECTDIR}/build/*.o
)

list(FILTER OBJS 
    EXCLUDE REGEX ${PROJECTDIR}/build/dhcp-stats.o
)

enable_testing()

add_executable(
    testing
    ip_addr_parsing_tests.cpp
)

target_link_libraries(
    testing    
    ${OBJS}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testing)
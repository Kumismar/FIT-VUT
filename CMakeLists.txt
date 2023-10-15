cmake_minimum_required(VERSION 3.14)
project(DhcpStats LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB SOURCE_FILES "src/*.cpp")
add_executable(dhcp-stats ${SOURCE_FILES})

target_compile_options(dhcp-stats PRIVATE -Wall -Wextra -Werror -g)
target_link_libraries(dhcp-stats pcap)

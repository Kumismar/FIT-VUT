# Author: Ondřej Koumar
# Date: 2024-03-18

set(project_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_library(ParserLibrary)

add_executable(Parser
        main.cpp
)

target_link_libraries(Parser
        ParserLibrary
)

target_include_directories(ParserLibrary PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lex
        ${CMAKE_CURRENT_SOURCE_DIR}/grammars
        ${CMAKE_CURRENT_SOURCE_DIR}/types
        ${CMAKE_CURRENT_SOURCE_DIR}/tables
        ${CMAKE_CURRENT_SOURCE_DIR}/parsers
        ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
        ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
        ${CMAKE_CURRENT_SOURCE_DIR}/logger
        ${CMAKE_CURRENT_SOURCE_DIR}/ast
)

target_include_directories(Parser PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lex
        ${CMAKE_CURRENT_SOURCE_DIR}/grammars
        ${CMAKE_CURRENT_SOURCE_DIR}/types
        ${CMAKE_CURRENT_SOURCE_DIR}/tables
        ${CMAKE_CURRENT_SOURCE_DIR}/parsers
        ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
        ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
        ${CMAKE_CURRENT_SOURCE_DIR}/logger
        ${CMAKE_CURRENT_SOURCE_DIR}/ast
)

add_subdirectory(lex)
add_subdirectory(grammars)
add_subdirectory(parsers)
add_subdirectory(tables)
add_subdirectory(types)
add_subdirectory(exceptions)
add_subdirectory(logger)
add_subdirectory(ast)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${project_dir}/build)

target_compile_options(ParserLibrary PRIVATE
        -Wall
        -Wextra
        -pedantic
        -std=c++20
        -Wno-unused-parameter
        -Wno-register
)

target_compile_options(Parser PRIVATE
        -Wall
        -Wextra
        -pedantic
        -std=c++20
        -Wno-unused-parameter
        -Wno-register
)
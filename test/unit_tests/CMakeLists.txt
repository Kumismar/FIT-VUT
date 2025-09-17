# Author: Ondřej Koumar
# Email: xkouma02@stud.fit.vutbr.cz
# Create Time: 2024-04-28 18:26

add_subdirectory(parser_aux_classes)
add_subdirectory(ast)
add_subdirectory(parsers)

target_sources(ParserTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
)
# Author: Ondřej Koumar
# Email: xkouma02@stud.fit.vutbr.cz
# Create Time: 2024-04-28 18:32

target_sources(ParserTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ast_node_factory_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ast_linking_tests.cpp
)
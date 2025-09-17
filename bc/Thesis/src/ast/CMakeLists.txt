# @ Author: Ondřej Koumar
# @ Email: xkouma02@stud.fit.vutbr.cz
# @ Create Time: 2024-04-05 10:05


target_include_directories(ParserLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/expression
    ${CMAKE_CURRENT_SOURCE_DIR}/statements
)

target_sources(ParserLibrary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ast.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ast_node_factory.cpp
)

add_subdirectory(expression)
add_subdirectory(statements)
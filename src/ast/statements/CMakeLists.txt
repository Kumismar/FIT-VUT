# @ Author: Ondřej Koumar
# @ Email: xkouma02@stud.fit.vutbr.cz
# @ Create Time: 2024-04-03 11:11

target_sources(ParserLibrary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/elseif_statement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/for_loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/function_definition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/if_statement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/return_statement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statement_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/while_loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/declaration.cpp
)
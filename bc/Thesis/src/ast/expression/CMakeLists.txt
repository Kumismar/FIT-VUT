# @ Author: Ondřej Koumar
# @ Email: xkouma02@stud.fit.vutbr.cz
# @ Create Time: 2024-04-03 11:14

target_sources(ParserLibrary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/binary_expression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/constant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/function_call.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/variable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unary_expression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/operator.cpp
)
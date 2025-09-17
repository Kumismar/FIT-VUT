#  Author: Ondřej Koumar
#  Email: xkouma02@stud.fit.vutbr.cz
#  Create Time: 2024-04-28 18:27
#  Modified time: 2024-04-28 18:27
 
target_sources(ParserTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/precedence_parser_tests_fail.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/precedence_parser_tests_success.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/predictive_parser_tests_fail.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/predictive_parser_tests_success.cpp
)

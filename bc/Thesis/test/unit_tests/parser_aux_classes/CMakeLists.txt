# Author: Ondřej Koumar
# Email: xkouma02@stud.fit.vutbr.cz
# Create Time: 2024-04-28 18:30

target_sources(ParserTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/end_insertor_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pattern_finder_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/expression_processor_tests.cpp
)
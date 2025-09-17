# @ Author: Ondřej Koumar
# @ Email: xkouma02@stud.fit.vutbr.cz
# @ Create Time: 2024-05-02 19:28



target_sources(ParserLibrary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/predictive_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/precedence_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/end_insertor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pattern_finder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/expression_processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/symbol_handler.cpp
)

set_target_properties(ParserLibrary PROPERTIES 
    PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/predictive_parser.hpp
    PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/precedence_parser.hpp
)
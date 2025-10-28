file (GLOB lexer_src_files *.cpp)

target_sources(REParserLib PRIVATE
    ${lexer_src_files}
)

add_subdirectory(include)
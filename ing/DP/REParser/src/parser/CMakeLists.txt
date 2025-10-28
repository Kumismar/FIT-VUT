file(GLOB parser_src_files *.cpp)

target_sources(REParserLib PRIVATE
    ${parser_src_files}
)

add_subdirectory(include)
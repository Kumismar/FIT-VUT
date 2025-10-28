add_library(REParserLib STATIC)

add_subdirectory(include)
add_subdirectory(exception)
add_subdirectory(ast)
add_subdirectory(lexer)
add_subdirectory(parser)

file(GLOB src_files "*.cpp")

add_executable(${PROJECT_NAME} ${src_files})

target_link_libraries(REParserLib PUBLIC
        lexer_headers
        parser_headers
        ast_headers
        exception_headers
        headers
)

target_link_libraries(REParser PRIVATE REParserLib)

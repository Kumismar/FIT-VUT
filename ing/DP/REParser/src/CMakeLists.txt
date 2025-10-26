add_subdirectory(include)
add_subdirectory(exception)

file(GLOB src_files "*.cpp")

add_executable(${PROJECT_NAME} ${src_files})

target_link_libraries(${PROJECT_NAME} PUBLIC
        headers
)
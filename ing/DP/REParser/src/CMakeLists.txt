add_subdirectory(include)

file(GLOB src_files "*.cpp")

add_executable(${PROJECT_NAME} ${src_files})

target_link_libraries(${PROJECT_NAME} PUBLIC
    headers
)
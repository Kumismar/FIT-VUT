include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(REParserTest)

add_subdirectory(unit_tests)

target_link_libraries(REParserTest PRIVATE
        Catch2::Catch2WithMain
        REParserLib
)

include(Catch)
catch_discover_tests(REParserTest)
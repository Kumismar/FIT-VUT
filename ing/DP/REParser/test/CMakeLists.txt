include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_executable(REParserTest)

add_subdirectory(unit_tests)

target_link_libraries(REParserTest PRIVATE
        GTest::gtest_main
        REParserLib
)

include(GoogleTest)
gtest_discover_tests(REParserTest)
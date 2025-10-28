file(GLOB test_sources *.cpp)

target_sources(REParserTest PRIVATE
        ${test_sources}
)
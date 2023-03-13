message(STATUS "Generating tests")

get_filename_component(TESTS_PATH "." ABSOLUTE)
file(GLOB_RECURSE TEST_SOURCES ${TESTS_PATH}/*.cpp)

link_libraries(compiler)

add_executable(test_lex main.cpp lex/cases.cpp)
target_link_libraries(test_lex PRIVATE Catch2::Catch2)

add_executable(test_parse main.cpp parse/cases.cpp)
target_link_libraries(test_parse PRIVATE Catch2::Catch2)

add_executable(example_ast ast/example.cpp)
add_executable(example_parse parse/example.cpp)
add_executable(example_interpreter interpreter/example.cpp)

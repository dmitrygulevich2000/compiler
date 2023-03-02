message(STATUS "Generating tests")

get_filename_component(TESTS_PATH "." ABSOLUTE)
file(GLOB_RECURSE TEST_SOURCES ${TESTS_PATH}/*.cpp)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE compiler)
target_link_libraries(tests PRIVATE Catch2::Catch2)

add_executable(example_ast ast/cases.cpp)
target_link_libraries(example_ast PRIVATE compiler)

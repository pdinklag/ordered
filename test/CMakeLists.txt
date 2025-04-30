set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules")
include(InitGitSubmodules)
add_subdirectory(random-permutation)

add_executable(test-static test_static.cpp)
target_link_libraries(test-static PRIVATE ordered)
add_test(static ${CMAKE_CURRENT_BINARY_DIR}/test-static)

add_executable(test-set test_set.cpp)
target_link_libraries(test-set PRIVATE ordered random-permutation)
add_test(set ${CMAKE_CURRENT_BINARY_DIR}/test-set)

add_executable(test-examples test_examples.cpp)
target_link_libraries(test-examples PRIVATE ordered)
add_test(examples ${CMAKE_CURRENT_BINARY_DIR}/test-examples)

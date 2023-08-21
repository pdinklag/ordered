# we need the iopp submodule, initialize it
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules")
#include(InitGitSubmodules)
#add_subdirectory(iopp)

add_executable(test-static test_static.cpp)
target_link_libraries(test-static PRIVATE btree)
add_test(static ${CMAKE_CURRENT_BINARY_DIR}/test-static)

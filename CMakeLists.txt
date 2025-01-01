cmake_minimum_required(VERSION 3.10)
project(memory_alloc C)

set(CMAKE_C_STANDARD 11)

# Main library
add_library(memalloc
        src/memalloc.c
)

# Main executable
add_executable(memory_alloc
        src/main.c
)
target_link_libraries(memory_alloc memalloc)

# Test executable
# Test executable
add_executable(memory_tests
        tests/test_main.c
        tests/allocation_tests/test_basic.c
        tests/allocation_tests/test_array.c
        tests/allocation_tests/test_edge_cases.c
        tests/allocation_tests/test_realloc.c
        tests/leak_tests/test_simple_leak.c
        tests/leak_tests/test_struct_leak.c
)
target_link_libraries(memory_tests memalloc)
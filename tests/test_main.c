#include "test_main.h"
#include <stdio.h>

int main() {
    printf("Running all tests...\n\n");

    // Run allocation tests
    printf("Running basic allocation tests:\n");
    run_basic_tests();
    printf("\n");

    printf("Running array tests:\n");
    run_array_tests();
    printf("\n");

    printf("Running edge case tests:\n");
    run_edge_case_tests();
    printf("\n");

    printf("Running realloc tests:\n");
    run_realloc_tests();
    printf("\n");

    // Run leak tests
    printf("Running leak tests:\n");
    run_leak_tests();
    printf("\n");

    printf("Running struct leak tests:\n");
    run_struct_leak_tests();
    printf("\n");

    printf("All tests completed!\n");
    return 0;
}
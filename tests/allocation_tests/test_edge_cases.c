// test_edge_cases.c
#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

void test_zero_size_malloc() {
    void* ptr = malloc(0);
    assert(ptr == NULL);
}

void test_large_allocation() {
    void* ptr = malloc(1024 * 1024 * 1024); // 1GB
    assert(ptr != NULL);
    free(ptr);
}

void test_invalid_free() {
    free(NULL);
}

void run_edge_case_tests() {
    test_zero_size_malloc();
    test_large_allocation();
    test_invalid_free();
    printf("Edge case tests passed!\n");
}
// test_realloc.c
#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

void test_shrink_realloc() {
    int* arr = (int*)malloc(10 * sizeof(int));
    arr = (int*)realloc(arr, 5 * sizeof(int));
    assert(arr != NULL);
    free(arr);
}

void test_expand_realloc() {
    int* arr = (int*)malloc(5 * sizeof(int));
    arr = (int*)realloc(arr, 10 * sizeof(int));
    assert(arr != NULL);
    free(arr);
}

void test_realloc_to_null() {
    int* arr = (int*)malloc(5 * sizeof(int));
    arr = (int*)realloc(arr, 0);
    assert(arr == NULL);
}

void run_realloc_tests() {
    test_shrink_realloc();
    test_expand_realloc();
    test_realloc_to_null();
    printf("Realloc tests passed!\n");
}
// test_basic.c
#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

void test_malloc_basic() {
    int* ptr = (int*)malloc(sizeof(int));
    assert(ptr != NULL);
    *ptr = 42;
    assert(*ptr == 42);
    free(ptr);
}

void test_calloc_basic() {
    int* arr = (int*)calloc(5, sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 5; i++) {
        assert(arr[i] == 0);
    }
    free(arr);
}

void test_realloc_basic() {
    int* arr = (int*)malloc(2 * sizeof(int));
    arr[0] = 1;
    arr[1] = 2;
    arr = (int*)realloc(arr, 4 * sizeof(int));
    assert(arr != NULL);
    arr[2] = 3;
    arr[3] = 4;
    for (int i = 0; i < 4; i++) {
        assert(arr[i] == i + 1);
    }
    free(arr);
}

void run_basic_tests() {
    test_malloc_basic();
    test_calloc_basic();
    test_realloc_basic();
    printf("Basic tests passed!\n");
}
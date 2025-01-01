// test_array.c
#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

void test_array_allocation() {
    int* arr = (int*)malloc(10 * sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 2;
    }
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == i * 2);
    }
    free(arr);
}

void test_array_reallocation() {
    int* arr = (int*)malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
    arr = (int*)realloc(arr, 10 * sizeof(int));
    for (int i = 5; i < 10; i++) {
        arr[i] = i * 2;
    }
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == (i < 5 ? i : i * 2));
    }
    free(arr);
}

void test_array_zero_allocation() {
    // Test both num = 0 and size = 0 cases
    int* arr1 = (int*)calloc(0, sizeof(int));
    int* arr2 = (int*)calloc(5, 0);
    assert(arr1 == NULL);
    assert(arr2 == NULL);
}

void run_array_tests() {
    test_array_allocation();
    test_array_reallocation();
    test_array_zero_allocation();
    printf("Array tests passed!\n");
}

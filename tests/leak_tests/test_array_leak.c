#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

void test_array_simple_leak() {
    int* arr = (int*)malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }
    // Deliberately not freeing
}

void test_array_realloc_leak() {
    int* arr = (int*)malloc(5 * sizeof(int));
    arr = (int*)realloc(arr, 10 * sizeof(int));
    // Deliberately not freeing expanded array
}

void test_array_partial_free() {
    int* arr1 = (int*)malloc(5 * sizeof(int));
    int* arr2 = (int*)malloc(5 * sizeof(int));
    int* arr3 = (int*)malloc(5 * sizeof(int));

    free(arr1);  // Free first array
    free(arr3);  // Free third array
    // Deliberately not freeing arr2
}

void test_array_no_leak() {
    int* arr = (int*)malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }
    free(arr);
}

void run_array_leak_tests() {
    test_array_simple_leak();
    test_array_realloc_leak();
    test_array_partial_free();
    test_array_no_leak();
    print_memory_leaks();
    printf("Array leak tests completed!\n");
}
#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

// Local test struct definition
struct TestStruct {
    int data;
    char buffer[64];
};

void test_struct_leak_single() {
    struct TestStruct* s = (struct TestStruct*)malloc(sizeof(struct TestStruct));
    (void)s;
}

void test_struct_leak_array() {
    struct TestStruct* arr = (struct TestStruct*)malloc(5 * sizeof(struct TestStruct));
    (void)arr;
}

void test_struct_leak_free() {
    struct TestStruct* s = (struct TestStruct*)malloc(sizeof(struct TestStruct));
    assert(s != NULL);
    free(s);
}

void run_struct_leak_tests() {
    test_struct_leak_single();
    test_struct_leak_array();
    test_struct_leak_free();
    print_memory_leaks();
    printf("Struct leak tests completed!\n");
}
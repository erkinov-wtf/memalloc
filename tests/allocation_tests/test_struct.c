// test_struct.c
#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

struct TestStruct {
    int a;
    double b;
    char c;
};

void test_struct_allocation() {
    struct TestStruct* s = (struct TestStruct*)malloc(sizeof(struct TestStruct));
    assert(s != NULL);
    s->a = 10;
    s->b = 20.5;
    s->c = 'z';
    assert(s->a == 10);
    assert(s->b == 20.5);
    assert(s->c == 'z');
    free(s);
}

void test_struct_array() {
    struct TestStruct* arr = (struct TestStruct*)calloc(5, sizeof(struct TestStruct));
    assert(arr != NULL);
    free(arr);
}

void test_struct_realloc() {
    struct TestStruct* arr = (struct TestStruct*)malloc(2 * sizeof(struct TestStruct));
    arr = (struct TestStruct*)realloc(arr, 5 * sizeof(struct TestStruct));
    assert(arr != NULL);
    free(arr);
}

void run_struct_tests() {
    test_struct_allocation();
    test_struct_array();
    test_struct_realloc();
    printf("Struct tests passed!\n");
}
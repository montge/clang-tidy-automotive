// RUN: %check_clang_tidy %s automotive-c23-req-11.5 %t

// Test: Pointer conversion rules (MISRA Rules 11.2, 11.5, 11.6, 11.7)

struct incomplete;

// Violation - void* to typed pointer (Rule 11.5)
void test_void_to_typed(void *vptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: conversion from void pointer to object pointer
    int *iptr = (int *)vptr;
    (void)iptr;
}

// Violation - void* to arithmetic (Rule 11.6)
void test_void_to_arithmetic(void *vptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: conversion from void pointer to arithmetic
    long val = (long)vptr;
    (void)val;
}

// Violation - arithmetic to void* (Rule 11.6)
void test_arithmetic_to_void(long val) {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: conversion from arithmetic type to void pointer
    void *vptr = (void *)val;
    (void)vptr;
}

// Violation - pointer to float (Rule 11.7)
void test_pointer_to_float(int *iptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: conversion from pointer to floating
    double d = (double)(unsigned long)iptr;
    (void)d;
}

// Compliant - cast between same types
void test_same_type(int *iptr) {
    int *another = (int *)iptr;
    (void)another;
}

// Compliant - explicit void* usage
void test_void_star(void) {
    int x = 42;
    void *vptr = &x;
    (void)vptr;
}

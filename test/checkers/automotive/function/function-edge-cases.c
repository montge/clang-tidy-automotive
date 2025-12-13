// Test file for: automotive function checks (Edge Cases)
// Related MISRA C:2025 Rules: 8.2, 8.3, 17.7, 17.8
//
// This file tests edge cases for function-related checks

// RUN: %check_clang_tidy %s automotive-uncomplete-function-prototype,automotive-missing-return-value-handling,automotive-avoid-function-parameter-modification %t

#include <stdlib.h>

//===----------------------------------------------------------------------===//
// Edge Case: Empty parameter list vs void (8.2)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: incomplete function prototype
int func_empty_params();  // Empty () means unspecified params

int func_void_params(void);  // Explicit void - OK

//===----------------------------------------------------------------------===//
// Edge Case: K&R style function definitions
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: incomplete function prototype
int old_style_func(a, b)  // K&R style
    int a;
    int b;
{
    return a + b;
}

//===----------------------------------------------------------------------===//
// Edge Case: Function pointer with incomplete prototype
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:16: warning: incomplete function prototype
typedef int (*callback_empty)();  // Empty params

typedef int (*callback_void)(void);  // OK

//===----------------------------------------------------------------------===//
// Edge Case: Return value handling (17.7)
//===----------------------------------------------------------------------===//

int get_value(void) { return 42; }
void *allocate(size_t size) { return malloc(size); }

void test_return_value_handling(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return value
    get_value();  // Return value ignored

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return value
    allocate(100);  // Return value ignored (memory leak)

    // Compliant: Return value used
    int x = get_value();
    void *p = allocate(100);
    (void)x;
    free(p);

    // Compliant: Explicitly cast to void
    (void)get_value();
}

//===----------------------------------------------------------------------===//
// Edge Case: Chained function calls
//===----------------------------------------------------------------------===//

int process(int x) { return x * 2; }
int transform(int x) { return x + 1; }

void test_chained_calls(void) {
    // Return value of inner call is used
    int result = process(transform(get_value()));
    (void)result;
}

//===----------------------------------------------------------------------===//
// Edge Case: Parameter modification (17.8)
//===----------------------------------------------------------------------===//

void modify_param_direct(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: modifying function parameter
    x = 10;  // Direct modification
}

void modify_param_increment(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: modifying function parameter
    x++;  // Increment modification
}

void modify_param_compound(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: modifying function parameter
    x += 5;  // Compound assignment
}

// Compliant: Using local copy
void use_local_copy(int x) {
    int local = x;
    local = 10;
    local++;
    local += 5;
    (void)local;
}

// Compliant: Pointer parameter pointing to modified data
void modify_pointee(int *x) {
    *x = 10;  // Modifying what x points to, not x itself - OK
}

//===----------------------------------------------------------------------===//
// Edge Case: Variadic function return value
//===----------------------------------------------------------------------===//

#include <stdio.h>

void test_variadic_return(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return value
    printf("Hello");  // printf returns int

    // Compliant
    int n = printf("Hello");
    (void)n;
}

//===----------------------------------------------------------------------===//
// Compliant Cases
//===----------------------------------------------------------------------===//

int compliant_func(int a, int b) {  // Named parameters
    return a + b;
}

void compliant_void_func(void) {  // Explicit void
    return;
}

void compliant_no_modify(int x) {  // Parameter not modified
    int y = x * 2;
    (void)y;
}

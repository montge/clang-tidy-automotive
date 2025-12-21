// RUN: %check_clang_tidy %s automotive-c23-req-17.13 %t
// Test for automotive-c23-req-17.13
// Related MISRA C:2025 Rule: 17.13
// Note: Function type with const/volatile qualification is an unusual pattern
// that most compilers reject or warn about. This test shows compliant patterns.

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Normal function pointer typedef
typedef int (*normal_func_ptr)(void);

// Pointer to function returning const
typedef const int *(*func_returning_const_ptr)(void);

// Function taking const parameter
typedef void (*func_with_const_param)(const int *);

// Regular function declaration
int regular_function(void);

void test_usage(void) {
    normal_func_ptr fp = regular_function;
    (void)fp;
}

// Note: The following would be a violation but is invalid C syntax:
// typedef void (const *func_ptr)(void);  // const-qualified function type

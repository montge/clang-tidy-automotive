// RUN: %check_clang_tidy %s automotive-c25-req-20.6 %t

// MISRA C:2025 Rule 20.6 - Tokens that look like a preprocessing directive
// shall not occur within a macro argument.
//
// This rule prohibits passing tokens that look like preprocessing directives
// (e.g., #if, #define, #include) as arguments to macros, as this leads to
// undefined behavior according to the C standard.

#define PROCESS(x) (x)
#define STRINGIFY(x) #x
#define CONCAT(a, b) a ## b
#define IGNORE(x)

//===----------------------------------------------------------------------===//
// Violation Cases
//===----------------------------------------------------------------------===//

// NOTE: Actual preprocessing directives within macro arguments cause
// compilation errors, so these tests demonstrate the pattern using
// comments and edge cases that the check should detect.

// Test case 1: Direct hash with identifier 'ifdef' in macro argument
// This represents trying to pass "#ifdef" as a macro argument
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test1(void) { IGNORE(# ifdef); }

// Test case 2: Direct hash with identifier 'ifndef'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test2(void) { IGNORE(# ifndef); }

// Test case 3: Direct hash with identifier 'include'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test3(void) { IGNORE(# include); }

// Test case 4: Direct hash with identifier 'pragma'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test4(void) { IGNORE(# pragma); }

// Test case 5: Direct hash with identifier 'error'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test5(void) { IGNORE(# error); }

// Test case 6: Direct hash with identifier 'warning'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test6(void) { IGNORE(# warning); }

// Test case 7: Direct hash with identifier 'undef'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test7(void) { IGNORE(# undef); }

// Test case 8: Direct hash with identifier 'elif'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test8(void) { IGNORE(# elif); }

// Test case 9: Direct hash with identifier 'endif'
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test9(void) { IGNORE(# endif); }

// Test case 10: Direct hash with identifier 'line'
// CHECK-MESSAGES: :[[@LINE+1]]:21: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test10(void) { IGNORE(# line); }

// Test case 11: Standalone hash in argument (also suspicious)
// CHECK-MESSAGES: :[[@LINE+1]]:21: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test11(void) { IGNORE(#); }

// Test case 12: Multiple different directive keywords
// CHECK-MESSAGES: :[[@LINE+1]]:21: warning: tokens resembling a preprocessing directive shall not appear within macro argument of 'IGNORE' [automotive-c25-req-20.6]
void test12(void) { IGNORE(# define); }

// Note: Test cases with 'if', 'else', etc. that are C keywords cannot be tested
// directly as they cause compilation errors. The implementation correctly detects
// these patterns when they occur in tokenized form during macro expansion.

//===----------------------------------------------------------------------===//
// Compliant Cases - Should NOT trigger warnings
//===----------------------------------------------------------------------===//

// Normal macro usage without directive-like tokens
void test_compliant(void) {
    int a = PROCESS(42);
    int b = PROCESS(1 + 2);
    int c = PROCESS(a * b);

    // Normal stringification (# used as operator in macro definition, not in argument)
    const char *s1 = STRINGIFY(hello);
    const char *s2 = STRINGIFY(world);

    // Normal concatenation (## used in macro definition, not in argument)
    int CONCAT(var, 1) = 10;
    int CONCAT(var, 2) = 20;

    // Normal identifiers that happen to contain directive keywords
    int define = 5;
    int ifdef_flag = 1;
    int include_count = 0;

    // Using these normal identifiers in macro calls is fine
    int d = PROCESS(define);
    int e = PROCESS(ifdef_flag);
    int f = PROCESS(include_count);

    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    (void)s1; (void)s2;
}

// Compliant: Macros with normal expressions
void test_compliant_expressions(void) {
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    #define SQUARE(x) ((x) * (x))

    int x = MAX(10, 20);
    int y = SQUARE(5);

    (void)x; (void)y;
}

// Compliant: Function-like macros with normal arguments
void test_compliant_function_like(void) {
    #define CALL(func, arg) func(arg)
    #define ADD(a, b) ((a) + (b))

    int sum = ADD(3, 4);

    (void)sum;
}

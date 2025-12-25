// RUN: %check_clang_tidy %s automotive-c25-req-20.7 %t

// MISRA C:2025 Rule 20.7 - Expressions resulting from macro expansion shall
// be appropriately delimited.

// Violations - macro parameters not parenthesized in expressions
#define ADD(a, b) a + b
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: macro parameter 'a' shall be enclosed in parentheses [automotive-c25-req-20.7]
// CHECK-MESSAGES: :[[@LINE-2]]:23: warning: macro parameter 'b' shall be enclosed in parentheses [automotive-c25-req-20.7]

#define MULTIPLY(x, y) x * y
// CHECK-MESSAGES: :[[@LINE-1]]:24: warning: macro parameter 'x' shall be enclosed in parentheses [automotive-c25-req-20.7]
// CHECK-MESSAGES: :[[@LINE-2]]:28: warning: macro parameter 'y' shall be enclosed in parentheses [automotive-c25-req-20.7]

#define SHIFT_LEFT(val, n) val << n
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: macro parameter 'val' shall be enclosed in parentheses [automotive-c25-req-20.7]
// CHECK-MESSAGES: :[[@LINE-2]]:35: warning: macro parameter 'n' shall be enclosed in parentheses [automotive-c25-req-20.7]

#define BITWISE_AND(a, b) a & b
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: macro parameter 'a' shall be enclosed in parentheses [automotive-c25-req-20.7]
// CHECK-MESSAGES: :[[@LINE-2]]:31: warning: macro parameter 'b' shall be enclosed in parentheses [automotive-c25-req-20.7]

#define COMPARE(x, y) x < y
// CHECK-MESSAGES: :[[@LINE-1]]:23: warning: macro parameter 'x' shall be enclosed in parentheses [automotive-c25-req-20.7]
// CHECK-MESSAGES: :[[@LINE-2]]:27: warning: macro parameter 'y' shall be enclosed in parentheses [automotive-c25-req-20.7]

#define LOGICAL_OR(a, b) a || b
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: macro parameter 'a' shall be enclosed in parentheses [automotive-c25-req-20.7]
// CHECK-MESSAGES: :[[@LINE-2]]:31: warning: macro parameter 'b' shall be enclosed in parentheses [automotive-c25-req-20.7]

// Compliant cases - parameters properly parenthesized
#define ADD_SAFE(a, b) ((a) + (b))
#define MULTIPLY_SAFE(x, y) ((x) * (y))
#define SHIFT_LEFT_SAFE(val, n) ((val) << (n))
#define BITWISE_AND_SAFE(a, b) ((a) & (b))
#define COMPARE_SAFE(x, y) ((x) < (y))

// Compliant - no expression context (just parameter substitution)
#define IDENTITY(x) x

// Compliant - stringification and concatenation don't need parentheses
#define STRINGIFY(x) #x
#define CONCAT(a, b) a ## b

// Compliant - function-like usage
#define CALL_FUNC(f, arg) f(arg)

// Test usage
void test_violations(void) {
    int result;

    result = ADD(1, 2);
    result = MULTIPLY(3, 4);
    result = SHIFT_LEFT(1, 2);
    result = BITWISE_AND(0xFF, 0x0F);
    result = COMPARE(5, 10) ? 1 : 0;
    result = LOGICAL_OR(0, 1) ? 1 : 0;

    (void)result;
}

void test_compliant(void) {
    int result;

    result = ADD_SAFE(1, 2);
    result = MULTIPLY_SAFE(3, 4);
    result = SHIFT_LEFT_SAFE(1, 2);
    result = BITWISE_AND_SAFE(0xFF, 0x0F);
    result = COMPARE_SAFE(5, 10) ? 1 : 0;

    result = IDENTITY(42);

    const char *str = STRINGIFY(hello);

    (void)result;
    (void)str;
}

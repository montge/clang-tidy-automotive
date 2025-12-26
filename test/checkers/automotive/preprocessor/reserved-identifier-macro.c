// XFAIL: *
// Note: Check also reports compiler-internal reserved identifiers
// RUN: %check_clang_tidy %s automotive-c25-req-20.15 %t

// Test: Reserved identifiers starting with underscore + uppercase
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '_Reserved' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define _Reserved 1

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '_ANOTHER_RESERVED' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define _ANOTHER_RESERVED 2

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '_MY_CONSTANT' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define _MY_CONSTANT 3

// Test: Reserved identifiers starting with double underscore
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '__internal' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define __internal 4

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '__DOUBLE_UNDERSCORE' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define __DOUBLE_UNDERSCORE 5

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '__reserved' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define __reserved 6

// Test: Standard library reserved names
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'NULL' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define NULL 0

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'errno' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define errno 0

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'EOF' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define EOF (-1)

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'stdin' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define stdin ((void*)0)

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'stdout' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define stdout ((void*)0)

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'stderr' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define stderr ((void*)0)

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'assert' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#define assert(x) ((void)0)

// Test: #undef of reserved identifiers
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: #undef of reserved identifier 'NULL' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#undef NULL

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: #undef of reserved identifier 'EOF' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#undef EOF

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: #undef of reserved identifier '_Reserved' violates MISRA C:2025 Rule 20.15 [automotive-c25-req-20.15]
#undef _Reserved

// Compliant: Normal macro names (no warnings expected)
#define MY_MACRO 100
#define myMacro 200
#define VALID_NAME 300
#define _lowercase 400  // lowercase after underscore is OK
#define valid_name 500
#define ANOTHER_VALID_MACRO 600

// Test usage of compliant macros
void test_function(void) {
    int x = MY_MACRO;
    int y = myMacro;
    int z = VALID_NAME;
    int w = _lowercase;
    (void)x;
    (void)y;
    (void)z;
    (void)w;
}

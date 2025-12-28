// RUN: %check_clang_tidy %s automotive-avoid-reserved-macro-identifier %t -- -- -undef

// Test: Reserved identifiers starting with underscore + uppercase
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '_Reserved' [automotive-avoid-reserved-macro-identifier]
#define _Reserved 1

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '_ANOTHER_RESERVED' [automotive-avoid-reserved-macro-identifier]
#define _ANOTHER_RESERVED 2

// Test: Reserved identifiers starting with double underscore
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '__internal' [automotive-avoid-reserved-macro-identifier]
#define __internal 3

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier '__DOUBLE_UNDERSCORE' [automotive-avoid-reserved-macro-identifier]
#define __DOUBLE_UNDERSCORE 4

// Test: Standard library reserved names
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'NULL' [automotive-avoid-reserved-macro-identifier]
#define NULL 0

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'errno' [automotive-avoid-reserved-macro-identifier]
#define errno 0

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: #define of reserved identifier 'EOF' [automotive-avoid-reserved-macro-identifier]
#define EOF (-1)

// Test: #undef of reserved identifiers
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: #undef of reserved identifier 'NULL' [automotive-avoid-reserved-macro-identifier]
#undef NULL

// Compliant: Normal macro names
#define MY_MACRO 100
#define myMacro 200
#define VALID_NAME 300
#define _lowercase 400  // lowercase after underscore is OK

void test_function(void) {
    int x = MY_MACRO;
    int y = myMacro;
    (void)x;
    (void)y;
}

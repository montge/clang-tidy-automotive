// RUN: %check_clang_tidy %s automotive-avoid-comment-within-comment %t -- -- -std=c11

// Test: Comment markers within comments (MISRA Rule 3.1)

// This is a compliant single-line comment

/* This is a compliant block comment */

// This comment contains /* which is not compliant
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: comment marker '/*' found within '//' comment

// This comment contains // which is also not compliant
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: comment marker '//' found within '//' comment

/* This block comment contains /* nested marker
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: comment marker '/*' found within '/* */' comment
*/

/* This block comment contains // marker
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: comment marker '//' found within '/* */' comment
*/

// URL-like patterns: http://example.com should be detected
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: comment marker '//' found within '//' comment

/* URL in block: https://example.com/path */
// CHECK-MESSAGES: :[[@LINE-1]]:24: warning: comment marker '//' found within '/* */' comment

// Compliant: no comment markers in this comment
/* Compliant: no comment markers in this block comment */

void test_code(void) {
    // Compliant comment inside function
    int x = 42;

    /* Another compliant
       multi-line
       block comment */

    (void)x;
}

// Edge case: /* at the very end */
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: comment marker '/*' found within '//' comment

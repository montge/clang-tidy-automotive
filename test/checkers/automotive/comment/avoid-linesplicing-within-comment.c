// RUN: %check_clang_tidy %s automotive-c25-req-3.3 %t -- -- -std=c11
// Test for automotive-c25-req-3.3: line-splicing shall not be used within comments

// This is a normal single-line comment - OK

/* This is a normal
   multi-line comment - OK */

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: line-splicing within comment
// This comment has a backslash at end \
   which continues the comment to the next line

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: line-splicing within comment
/* Comment with backslash-newline \
   inside a block comment */

// Normal comments without line splicing are OK
// No backslash at the end

/* Multi-line comment
 * without line splicing
 * is compliant
 */

void function_with_comments(void) {
  // Regular comment inside function - OK

  int x = 10;  // Inline comment - OK
  (void)x;

  /* Block comment inside function - OK */
}

// File ends with a normal comment - OK

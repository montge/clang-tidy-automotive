// RUN: %check_clang_tidy %s automotive-avoid-line-splicing-within-comment %t -- -- -std=c11
// Test for automotive-avoid-line-splicing-within-comment: line-splicing shall not be used within comments

// This is a normal single-line comment - OK

/* This is a normal
   multi-line comment - OK */

// CHECK-MESSAGES: :[[@LINE+1]]:40: warning: avoid line-spliceing within a '//' comment [automotive-avoid-line-splicing-within-comment]
// This comment has a backslash at end \
   which continues the comment to the next line

// Note: The check currently only detects // comments, not /* */ block comments

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

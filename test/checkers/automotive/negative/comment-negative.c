// RUN: clang-tidy %s --checks='-*,automotive-avoid-comment-within-comment,automotive-avoid-line-splicing-within-comment' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger comment warnings.

// Simple single-line comment
// This is a proper comment

// Multiple single-line comments
// Line 1
// Line 2
// Line 3

/* Proper block comment */

/*
 * Multi-line block comment
 * with proper formatting
 * no nested comments
 */

/**
 * Documentation comment style
 * @param x the input value
 * @return the computed result
 */
int documented_function(int x) {
  return x * 2;
}

// Code with inline comments
void example_function(void) {
  int a = 10;  // Initialize a
  int b = 20;  // Initialize b
  int c = a + b;  // Compute sum
  (void)c;
}

/* Comments with special characters that are allowed */
/* Path examples: home user file.txt */
/* Math: a divided by b equals c */

// CHECK-NOT: warning:

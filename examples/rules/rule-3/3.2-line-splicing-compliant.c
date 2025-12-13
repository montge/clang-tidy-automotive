// MISRA C:2025 Rule 3.2 - Line-splicing in comments
// Category: Required
// Status: COMPLIANT
//
// No line-splicing used in single-line comments.

// COMPLIANT: No backslash at end of comment

// COMPLIANT: Multi-line comments use block style instead
/* This is a multi-line comment
   that spans several lines
   without using line splicing */

// COMPLIANT: Or use multiple single-line comments
// This is line one of a longer explanation
// This is line two of the explanation
// This is line three of the explanation

void example(void) {
    int x = 1;
    // Debug: print value
    // printf("x = %d\n", x);  // Properly commented out
    (void)x;
}

// COMPLIANT: If you need to show a backslash, don't put it at line end
// Path example: C:\Users\Name\Documents
// The backslash above is not at the end, so no line splicing occurs

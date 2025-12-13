// MISRA C:2025 Rule 3.2 - Line-splicing in comments
// Category: Required
// Status: VIOLATION
//
// Line-splicing shall not be used in // comments.
//
// Expected diagnostic:
//   warning: line splicing in comment [automotive-avoid-line-splicing-within-comment]

// VIOLATION: Backslash at end of line-comment continues to next line \
This line is still part of the comment above!

// VIOLATION: This can accidentally comment out code \
int important_variable = 42;

// The backslash-newline sequence joins lines during preprocessing,
// which can lead to unexpected behavior where the next line
// becomes part of the comment.

// VIOLATION: Even with spaces after backslash (some compilers) \
int another_variable = 0;

void example(void) {
    int x = 1;
    // Debug: print value \
    printf("x = %d\n", x);  // This printf is commented out!
    (void)x;
}

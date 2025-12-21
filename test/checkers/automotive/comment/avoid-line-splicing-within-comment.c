// RUN: %check_clang_tidy %s automotive-avoid-line-splicing-within-comment %t -- -- -std=c11

// Test: Line splicing in // comments (MISRA Rule 3.2)

// This is a compliant comment - no line splicing

// This comment has a line splice \
   which continues the comment on the next line - NOT compliant
// CHECK-MESSAGES: :[[@LINE-2]]:35: warning: avoid line-spliceing within a '//' comment
// CHECK-MESSAGES: :[[@LINE-3]]:35: warning: multi-line // comment [clang-diagnostic-comment]

// Multiple line splices \
   first continuation \
   second continuation - NOT compliant
// CHECK-MESSAGES: :[[@LINE-3]]:26: warning: avoid line-spliceing within a '//' comment
// CHECK-MESSAGES: :[[@LINE-4]]:26: warning: multi-line // comment [clang-diagnostic-comment]
// CHECK-MESSAGES: :[[@LINE-4]]:23: warning: avoid line-spliceing within a '//' comment
// CHECK-MESSAGES: :[[@LINE-5]]:23: warning: multi-line // comment [clang-diagnostic-comment]

// Comment with trailing whitespace after backslash (\) - NOT a splice if whitespace

/\
/ This creates a comment with spliced slashes - NOT compliant
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: avoid line-spliceing within a '//' comment

/* Block comments can have line splices \
   this is allowed because the standard permits it for block comments */

// Compliant: backslash not at end of line
// path\to\file is fine because backslash is followed by other chars

void test_function(void) {
    // Compliant: comment inside function
    int x = 0;
    (void)x;
}

// Compliant: trigraph that looks like backslash (??/) followed by newline
// Note: Trigraphs are a separate issue (Rule 4.2)

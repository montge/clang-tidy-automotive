// XFAIL: *
// XFAIL: Check needs implementation
// RUN: %check_clang_tidy %s automotive-c25-req-20.6 %t -- -- -std=c11
// Test for automotive-c25-req-20.6: preprocessor directives in macro arguments

#define MACRO(x) (x)
#define MULTI_ARG(a, b) ((a) + (b))

// CHECK-MESSAGES: :[[@LINE+2]]:1: warning: preprocessing directive in macro argument
int result = MACRO(
#ifdef DEBUG
1
#else
0
#endif
);

// CHECK-MESSAGES: :[[@LINE+2]]:1: warning: preprocessing directive in macro argument
int value = MULTI_ARG(1,
#if 1
2
#endif
);

// OK - no preprocessor directives in arguments
int normal = MACRO(42);
int normal2 = MULTI_ARG(1, 2);

// OK - conditional compilation around macro call
#ifdef FEATURE
int feature = MACRO(100);
#endif

// OK - macro expansion without directive in argument
#define VALUE 10
int expanded = MACRO(VALUE);

void function(void) {
  int x = MACRO(5);  // OK - simple argument
  int y = MULTI_ARG(x, 10);  // OK - variable as argument
  (void)y;
}

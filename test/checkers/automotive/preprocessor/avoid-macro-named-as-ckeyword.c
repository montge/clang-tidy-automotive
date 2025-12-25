// RUN: %check_clang_tidy %s automotive-avoid-macro-named-as-ckeyword %t -- -- -std=c11
// Test for automotive-avoid-macro-named-as-ckeyword: macros shall not be named as C keywords

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define if 1

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define while something

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define return x

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define int myint

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define void myvoid

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define struct mystruct

// OK - normal macro names
#define MY_CONSTANT 42
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define DEBUG_MODE 1
#define BUFFER_SIZE 1024

// OK - similar but not exact keyword matches
#define IF_CONDITION 1
#define WHILE_LOOP 1
#define RETURN_VALUE 0
#define INTEGER_TYPE int

void use_macros(void) {
  int x = MY_CONSTANT;
  int y = MAX(1, 2);
  (void)x;
  (void)y;
}

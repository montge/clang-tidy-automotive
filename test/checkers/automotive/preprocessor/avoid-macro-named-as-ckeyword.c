// RUN: %check_clang_tidy %s automotive-avoid-macro-named-as-ckeyword %t -- -- -std=c11
// Test for automotive-avoid-macro-named-as-ckeyword: macros shall not be named as C keywords

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'if' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define if 1

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'while' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define while something

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'return' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define return x

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'int' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define int myint

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'void' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define void myvoid

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'struct' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define struct mystruct

// Undefine the keyword macros to allow compliant code below
#undef if
#undef while
#undef return
#undef int
#undef void
#undef struct

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

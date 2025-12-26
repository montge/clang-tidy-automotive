// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-19.0.3 %t -- -- -std=c++17
// Test for automotive-cpp23-req-19.0.3: macros shall not be named after C/C++ keywords

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define int myint

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name conflicts with C keyword
#define while something

// OK - normal macro names
#define MY_CONSTANT 42
#define INTEGER_TYPE int

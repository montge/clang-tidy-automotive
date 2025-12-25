// RUN: %check_clang_tidy %s automotive-cpp23-req-19.0.2 %t -- -- -std=c++17
// Test for automotive-cpp23-req-19.0.2: macro identifiers shall not be reserved

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro identifier is reserved
#define _RESERVED 1

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro identifier is reserved
#define __DOUBLE_UNDERSCORE 2

// OK - normal macro names
#define MY_MACRO 42
#define MAX_SIZE 100

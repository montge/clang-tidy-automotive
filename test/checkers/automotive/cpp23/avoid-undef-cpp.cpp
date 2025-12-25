// RUN: %check_clang_tidy %s automotive-cpp23-adv-19.0.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-19.0.1: #undef should not be used

#define MY_MACRO 1

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef
#undef MY_MACRO

// OK - macros without undef
#define CONSTANT 42
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// RUN: %check_clang_tidy %s automotive-cpp23-adv-19.3.3 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-19.3.3: # and ## operators should not be used

// CHECK-MESSAGES: :[[@LINE+1]]:22: warning: avoid preprocessor operator '#'
#define STRINGIFY(x) #x

// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: avoid preprocessor operator '##'
#define CONCAT(a, b) a##b

// OK - macros without # or ##
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define VALUE 42

// RUN: %check_clang_tidy %s automotive-cpp23-req-19.3.4 %t -- -- -std=c++17
// Test for automotive-cpp23-req-19.3.4: macro parameters shall be enclosed in parentheses

// CHECK-MESSAGES: :[[@LINE+2]]:19: warning: macro parameter 'x' shall be enclosed in parentheses [automotive-cpp23-req-19.3.4]
// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: macro parameter 'x' shall be enclosed in parentheses [automotive-cpp23-req-19.3.4]
#define BAD_SQ(x) x * x

// OK - parenthesized arguments
#define GOOD_SQ(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void test() {
  int result = GOOD_SQ(2 + 3);  // OK - properly parenthesized
  (void)result;
}

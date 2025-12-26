// RUN: %check_clang_tidy %s automotive-cpp23-req-21.2.4 %t -- -- -std=c++17
// Test for automotive-cpp23-req-21.2.4: offsetof macro shall not be used

// Define size_t and offsetof since test runs with -nostdinc++
using size_t = decltype(sizeof(0));
#define offsetof(type, member) __builtin_offsetof(type, member)

struct Data {
  int x;
  int y;
};

void test_offsetof() {
  // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: the macro offsetof shall not be used [automotive-cpp23-req-21.2.4]
  size_t offset = offsetof(Data, y);
  (void)offset;
}

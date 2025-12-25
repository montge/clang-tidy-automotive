// RUN: %check_clang_tidy %s automotive-cpp23-mand-25.5.2 %t -- -- -std=c++17
// Test for automotive-cpp23-mand-25.5.2: getenv/setlocale pointers shall be const

#include <cstdlib>
#include <clocale>

void test_getenv() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: pointer from getenv shall only be used as pointer to const
  char* env = std::getenv("PATH");
  
  // OK - const pointer
  const char* env_const = std::getenv("HOME");
  (void)env;
  (void)env_const;
}

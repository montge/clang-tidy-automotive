// RUN: %check_clang_tidy %s automotive-cpp23-req-25.5.1 %t -- -- -std=c++17
// Test for automotive-cpp23-req-25.5.1: localeconv pointer shall be const

#include <clocale>

void test_localeconv() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: pointer from localeconv shall only be used as pointer to const
  struct lconv* lc = std::localeconv();
  
  // OK - const pointer
  const struct lconv* lc_const = std::localeconv();
  (void)lc;
  (void)lc_const;
}

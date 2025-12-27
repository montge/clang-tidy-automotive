// RUN: %check_clang_tidy %s automotive-cpp23-req-25.5.1 %t -- -- -std=c++17
// Test for automotive-cpp23-req-25.5.1: localeconv pointer shall be const

extern "C" struct lconv *localeconv();

void test_localeconv() {
  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: pointer returned by 'localeconv' shall only be used as pointer to const-qualified type
  struct lconv* lc = localeconv();

  // OK - const pointer
  const struct lconv* lc_const = localeconv();
  (void)lc;
  (void)lc_const;
}

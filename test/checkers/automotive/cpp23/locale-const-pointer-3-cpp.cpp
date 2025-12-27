// RUN: %check_clang_tidy %s automotive-cpp23-mand-25.5.3 %t -- -- -std=c++17
// Test for automotive-cpp23-mand-25.5.3: strerror pointer shall be const

extern "C" char *strerror(int errnum);

void test_strerror() {
  // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: pointer returned by 'strerror' shall only be used as pointer to const-qualified type
  char* err = strerror(1);

  // OK - const pointer
  const char* err_const = strerror(2);
  (void)err;
  (void)err_const;
}

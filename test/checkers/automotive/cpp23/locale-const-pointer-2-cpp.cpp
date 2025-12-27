// RUN: %check_clang_tidy %s automotive-cpp23-mand-25.5.2 %t -- -- -std=c++17
// Test for automotive-cpp23-mand-25.5.2: getenv/setlocale pointers shall be const

extern "C" char *getenv(const char *name);

void test_getenv() {
  // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: pointer returned by 'getenv' shall only be used as pointer to const-qualified type
  char* env = getenv("PATH");

  // OK - const pointer
  const char* env_const = getenv("HOME");
  (void)env;
  (void)env_const;
}

// RUN: %check_clang_tidy %s automotive-missing-static-inline %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-missing-static-inline %t -- -- -std=c11

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inline function shall be static [automotive-missing-static-inline]
inline int f() {
  return 42;
}

static inline int g() {
  return 0;
}

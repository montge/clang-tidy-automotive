// RUN: %check_clang_tidy %s automotive-cpp23-req-21.2.3 %t
// Test for automotive-cpp23-req-21.2.3: system() from <cstdlib> shall not be used

#include <stdlib.h>

void test_system() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: Avoid 'system' call from stdlib
  system("ls");
}

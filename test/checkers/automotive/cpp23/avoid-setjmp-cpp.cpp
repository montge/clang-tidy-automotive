// RUN: clang-tidy %s --checks='-*,automotive-cpp23-req-21.10.2' -- 2>&1 | FileCheck %s
// Test for automotive-cpp23-req-21.10.2: <csetjmp> header shall not be used

// CHECK: warning: inclusion of <setjmp.h> is not allowed in safety-critical code
#include <setjmp.h>

jmp_buf env;

void test_setjmp() {
  // CHECK: warning: use of 'setjmp' is not allowed in safety-critical code
  if (setjmp(env) != 0) {
    // returned from longjmp
  }
}

void test_longjmp() {
  // CHECK: warning: use of 'longjmp' is not allowed in safety-critical code
  longjmp(env, 1);
}

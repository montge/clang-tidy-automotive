// RUN: %check_clang_tidy %s automotive-cpp23-req-8.4.1 %t
// Test for automotive-cpp23-req-8.4.1: Narrowing conversions

void test_narrowing() {
  double d = 3.14;
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: narrowing conversion from 'double' to 'int'
  int x = d;

  long long big = 1234567890123LL;
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: narrowing conversion from 'long long' to 'int'
  int y = big;

  unsigned u = 100;
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: narrowing conversion from 'unsigned int' to 'int'
  int z = u;

  (void)x;
  (void)y;
  (void)z;
}

void test_compliant() {
  int i = 42;
  double d = i;  // widening - OK
  long l = i;    // widening - OK
  (void)d;
  (void)l;
}

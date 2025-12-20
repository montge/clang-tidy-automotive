// RUN: %check_clang_tidy %s automotive-cpp23-req-8.2.4 %t
// Test for automotive-cpp23-req-8.2.4: reinterpret_cast shall not be used

void test_reinterpret_cast() {
  int x = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: reinterpret_cast shall not be used
  float* fp = reinterpret_cast<float*>(&x);
  (void)fp;

  long addr = 0x1000;
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: reinterpret_cast shall not be used
  int* ptr = reinterpret_cast<int*>(addr);
  (void)ptr;

  char buffer[4];
  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: reinterpret_cast shall not be used
  int* ip = reinterpret_cast<int*>(buffer);
  (void)ip;
}

void test_compliant_casts() {
  // static_cast is allowed
  double d = 3.14;
  int i = static_cast<int>(d);  // OK
  (void)i;

  // const_cast is a different rule (8.2.3)
  const int ci = 10;
  int* pi = const_cast<int*>(&ci);  // Different warning
  (void)pi;
}

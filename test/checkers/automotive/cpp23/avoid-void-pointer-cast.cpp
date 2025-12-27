// RUN: %check_clang_tidy %s automotive-cpp23-req-10.2 %t -- -- -std=c++17

// Test for automotive-cpp23-req-10.2: Conversions from void* to typed pointer

void test_static_cast_from_void_ptr() {
  int x = 42;
  void* vp = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: static_cast from void* to 'int'
  int* ip = static_cast<int*>(vp);
  (void)ip;
}

void test_reinterpret_cast_from_void_ptr() {
  float f = 3.14f;
  void* vp = &f;

  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: reinterpret_cast from void* to 'float'
  float* fp = reinterpret_cast<float*>(vp);
  (void)fp;
}

void test_c_style_cast_from_void_ptr() {
  long l = 100L;
  void* vp = &l;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: C-style cast from void* to 'long'
  long* lp = (long*)vp;
  (void)lp;
}

// Compliant cases - casting TO void* is OK
void test_compliant_to_void_ptr() {
  int x = 42;
  void* vp = static_cast<void*>(&x);  // OK - casting TO void*
  (void)vp;
}

// Test with user-defined types
struct MyStruct {
  int x;
  double y;
};

void test_user_defined_types() {
  MyStruct s{1, 2.0};
  void* vp = &s;

  // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: static_cast from void* to 'MyStruct'
  MyStruct* sp = static_cast<MyStruct*>(vp);
  (void)sp;
}

// Test with const void*
void test_const_void_ptr() {
  const int x = 42;
  const void* cvp = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: static_cast from void* to 'const int'
  const int* cip = static_cast<const int*>(cvp);
  (void)cip;
}

// Casting to character types
void test_char_ptr_casts() {
  int x = 42;
  void* vp = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: static_cast from void* to 'char'
  char* cp = static_cast<char*>(vp);
  (void)cp;

  // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: static_cast from void* to 'unsigned char'
  unsigned char* ucp = static_cast<unsigned char*>(vp);
  (void)ucp;
}

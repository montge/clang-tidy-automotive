// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-10.2 %t

// Test for automotive-cpp23-req-10.2: Conversions from void* to typed pointer

#include <cstdlib>
#include <new>

void test_static_cast_from_void_ptr() {
  void* vp = std::malloc(sizeof(int));

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: static_cast from void* to int is not verified and may be unsafe; ensure pointer validity and type compatibility [automotive-cpp23-req-10.2]
  int* ip = static_cast<int*>(vp);
  (void)ip;

  void* buffer = operator new(sizeof(double));
  // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: static_cast from void* to double is not verified and may be unsafe
  double* dp = static_cast<double*>(buffer);
  (void)dp;

  std::free(vp);
  operator delete(buffer);
}

void test_reinterpret_cast_from_void_ptr() {
  void* vp = std::malloc(sizeof(float));

  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: reinterpret_cast from void* to float is not verified and may be unsafe
  float* fp = reinterpret_cast<float*>(vp);
  (void)fp;

  std::free(vp);
}

void test_c_style_cast_from_void_ptr() {
  void* vp = std::malloc(sizeof(long));

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: C-style cast from void* to long is not verified and may be unsafe; use C++ cast with type verification [automotive-cpp23-req-10.2]
  long* lp = (long*)vp;
  (void)lp;

  std::free(vp);
}

void test_const_cast_from_void_ptr() {
  const void* cvp = std::malloc(sizeof(int));

  // This involves both const_cast and potentially void* conversion
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: const_cast from void* to int is not verified and may be unsafe
  int* ip = const_cast<int*>(static_cast<const int*>(cvp));
  (void)ip;

  std::free(const_cast<void*>(cvp));
}

// Compliant cases - typed allocation
void test_compliant_typed_allocation() {
  // Direct typed allocation - OK, no void* involved
  int* ip = new int;
  delete ip;

  int* ip2 = new int[10];
  delete[] ip2;

  // Placement new is also typed
  alignas(int) char buffer[sizeof(int)];
  int* ip3 = new(buffer) int;
  ip3->~int();
}

// Compliant cases - casting to/from different pointer types
void test_compliant_other_casts() {
  int x = 42;

  // Casting TO void* is OK (not FROM void*)
  void* vp = static_cast<void*>(&x);
  (void)vp;

  // Casting between non-void pointer types
  float f = 3.14f;
  void* vp2 = &f;

  // This is still a cast FROM void*, should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: static_cast from void* to float is not verified and may be unsafe
  float* fp = static_cast<float*>(vp2);
  (void)fp;
}

// Nested casts
void test_nested_casts() {
  void* vp = std::malloc(sizeof(int));

  // The inner cast from void* should be flagged
  // CHECK-MESSAGES: :[[@LINE+1]]:38: warning: static_cast from void* to int is not verified and may be unsafe
  const int* cip = static_cast<const int*>(static_cast<int*>(vp));
  (void)cip;

  std::free(vp);
}

// Test with user-defined types
struct MyStruct {
  int x;
  double y;
};

void test_user_defined_types() {
  void* vp = std::malloc(sizeof(MyStruct));

  // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: static_cast from void* to MyStruct is not verified and may be unsafe
  MyStruct* sp = static_cast<MyStruct*>(vp);
  (void)sp;

  std::free(vp);
}

// Test with const void*
void test_const_void_ptr() {
  const void* cvp = std::malloc(sizeof(int));

  // Casting from const void* to const int* - still should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: static_cast from void* to const int is not verified and may be unsafe
  const int* cip = static_cast<const int*>(cvp);
  (void)cip;

  std::free(const_cast<void*>(cvp));
}

// Function returning void*
void* allocate_buffer(size_t size) {
  return std::malloc(size);
}

void test_function_returning_void_ptr() {
  void* buffer = allocate_buffer(sizeof(double));

  // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: static_cast from void* to double is not verified and may be unsafe
  double* dp = static_cast<double*>(buffer);
  (void)dp;

  std::free(buffer);
}

// Casting to character types (typically used for byte access)
void test_char_ptr_casts() {
  void* vp = std::malloc(100);

  // Even char* casts should be flagged for consistency
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: static_cast from void* to char is not verified and may be unsafe
  char* cp = static_cast<char*>(vp);
  (void)cp;

  // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: static_cast from void* to unsigned char is not verified and may be unsafe
  unsigned char* ucp = static_cast<unsigned char*>(vp);
  (void)ucp;

  std::free(vp);
}

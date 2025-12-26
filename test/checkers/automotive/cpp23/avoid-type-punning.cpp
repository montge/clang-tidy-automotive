// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-5.13 %t
// Test for automotive-cpp23-req-5.13: Type-punning prevention

#include <cstring>

struct A {
  int value;
};

struct B {
  int value;
};

class Base {
public:
  virtual ~Base() = default;
  int x;
};

class Derived : public Base {
public:
  int y;
};

void test_reinterpret_cast_dereference() {
  int x = 42;

  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: type-punning through dereferencing reinterpret_cast from int to float results in undefined behavior
  // CHECK-MESSAGES: :[[@LINE+1]]:15: note: cast occurs here
  *reinterpret_cast<float*>(&x) = 3.14f;

  float f = 1.5f;
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: type-punning through dereferencing reinterpret_cast from float to int results in undefined behavior
  // CHECK-MESSAGES: :[[@LINE+1]]:13: note: cast occurs here
  int val = *reinterpret_cast<int*>(&f);
  (void)val;
}

void test_reinterpret_cast_member_access() {
  A obj_a{10};

  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: type-punning through member access through reinterpret_cast from A to B results in undefined behavior
  // CHECK-MESSAGES: :[[@LINE+1]]:27: note: cast occurs here
  int bad = reinterpret_cast<B*>(&obj_a)->value;
  (void)bad;
}

void test_reinterpret_cast_array_subscript() {
  int buffer[10] = {0};

  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: type-punning through array subscript through reinterpret_cast from int to float results in undefined behavior
  // CHECK-MESSAGES: :[[@LINE+1]]:18: note: cast occurs here
  float first = reinterpret_cast<float*>(buffer)[0];
  (void)first;
}

void test_c_style_cast_dereference() {
  long addr = 0x1234;

  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: type-punning through dereferencing C-style cast from long to int results in undefined behavior
  // CHECK-MESSAGES: :[[@LINE+1]]:14: note: cast occurs here
  int data = *(int*)addr;
  (void)data;

  double d = 3.14159;
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: type-punning through dereferencing C-style cast from double to long results in undefined behavior
  // CHECK-MESSAGES: :[[@LINE+1]]:19: note: cast occurs here
  long bits = *(long*)&d;
  (void)bits;
}

void test_c_style_cast_member_access() {
  A obj{100};

  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: type-punning through member access through C-style cast from A to B results in undefined behavior
  // CHECK-MESSAGES: :[[@LINE+1]]:18: note: cast occurs here
  int result = ((B*)&obj)->value;
  (void)result;
}

// Compliant cases - should NOT trigger warnings
void test_compliant_char_pointer() {
  int x = 42;

  // OK - char* can alias anything
  char* cp = reinterpret_cast<char*>(&x);
  char c = *cp;
  (void)c;

  // OK - unsigned char* can alias anything
  unsigned char* ucp = reinterpret_cast<unsigned char*>(&x);
  unsigned char uc = *ucp;
  (void)uc;

  // OK - signed char* can alias anything
  signed char* scp = reinterpret_cast<signed char*>(&x);
  signed char sc = *scp;
  (void)sc;
}

void test_compliant_void_pointer() {
  int x = 42;

  // OK - void* is compatible with any pointer
  void* vp = reinterpret_cast<void*>(&x);
  (void)vp;
}

void test_compliant_memcpy() {
  int x = 42;
  float f;

  // OK - using memcpy for type reinterpretation
  std::memcpy(&f, &x, sizeof(f));
  (void)f;
}

void test_compliant_base_derived() {
  Derived d;
  d.x = 10;
  d.y = 20;

  // OK - accessing derived through base pointer (inheritance relationship)
  Base* bp = reinterpret_cast<Base*>(&d);
  int val = bp->x;
  (void)val;

  // OK - accessing base through derived pointer (inheritance relationship)
  Base b;
  b.x = 5;
  Derived* dp = reinterpret_cast<Derived*>(&b);
  int val2 = dp->x;
  (void)val2;
}

void test_compliant_same_type() {
  int x = 42;

  // OK - same type after removing qualifiers
  const int* cip = &x;
  int* ip = reinterpret_cast<int*>(const_cast<int*>(cip));
  int val = *ip;
  (void)val;
}

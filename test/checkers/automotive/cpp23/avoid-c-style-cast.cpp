// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-8.2.1 %t

// Test C-style casts in C++ code

void test_c_style_casts() {
  int i = 42;
  double d = 3.14;

  // C-style casts - should warn
  double d2 = (double)i;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]

  int i2 = (int)d;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: C-style casts shall not be used

  long long_val = 42;
  void* ptr = (void*)long_val;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: C-style casts shall not be used

  // C++ casts - should not warn
  double d3 = static_cast<double>(i);
  int i3 = static_cast<int>(d);
  void* ptr2 = reinterpret_cast<void*>(i);
}

class Base {
public:
  virtual void foo() {}
};

class Derived : public Base {
public:
  void bar() {}
};

void test_class_casts() {
  Derived* derived = new Derived();

  // C-style cast - should warn
  Base* base = (Base*)derived;
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: C-style casts shall not be used

  // C++ cast - should not warn
  Base* base2 = static_cast<Base*>(derived);

  delete derived;
}

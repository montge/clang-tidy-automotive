// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-10.1 %t

// Test pointer conversions between unrelated classes using reinterpret_cast
// Note: static_cast between unrelated classes is already a compiler error

class Base1 {
public:
  int x;
  virtual ~Base1() {}
};

class Base2 {
public:
  int z;
  virtual ~Base2() {}
};

class Unrelated {
public:
  int w;
};

void test_reinterpret_cast() {
  Base1* b1 = new Base1();
  Base2* b2 = new Base2();
  Unrelated* u = new Unrelated();

  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: pointer conversion between unrelated classes 'Base1' and 'Base2' should be avoided
  Base2* ptr1 = reinterpret_cast<Base2*>(b1);

  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: pointer conversion between unrelated classes 'Base2' and 'Base1' should be avoided
  Base1* ptr2 = reinterpret_cast<Base1*>(b2);

  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: pointer conversion between unrelated classes 'Base1' and 'Unrelated' should be avoided
  Unrelated* ptr3 = reinterpret_cast<Unrelated*>(b1);

  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: pointer conversion between unrelated classes 'Unrelated' and 'Base1' should be avoided
  Base1* ptr4 = reinterpret_cast<Base1*>(u);

  (void)ptr1; (void)ptr2; (void)ptr3; (void)ptr4;
  delete b1;
  delete b2;
  delete u;
}

// Test with non-class pointer types (should not warn)
void test_non_class_pointers() {
  int* ip = new int(42);
  void* vp = static_cast<void*>(ip);  // OK - not class types
  int* ip2 = static_cast<int*>(vp);   // OK - not class types
  (void)ip2;
  delete ip;
}

// Test same class conversion (should not warn)
void test_same_class() {
  Base1* b1 = new Base1();
  Base1* b2 = reinterpret_cast<Base1*>(b1);  // OK - same class
  (void)b2;
  delete b1;
}

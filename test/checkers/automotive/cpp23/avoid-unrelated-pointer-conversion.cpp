// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-10.1 %t

// Test pointer conversions between unrelated classes

class Base1 {
public:
  int x;
  virtual ~Base1() {}
};

class Derived1 : public Base1 {
public:
  int y;
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

void test_static_cast() {
  Base1* b1 = new Base1();
  Derived1* d1 = new Derived1();
  Base2* b2 = new Base2();
  Unrelated* u = new Unrelated();

  // Valid: derived to base
  Base1* ptr1 = static_cast<Base1*>(d1);  // OK

  // Valid: base to derived (downcast)
  Derived1* ptr2 = static_cast<Derived1*>(b1);  // OK

  // Invalid: unrelated classes
  Base2* ptr3 = static_cast<Base2*>(b1);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: pointer conversion between unrelated classes 'Base1' and 'Base2' should be avoided [automotive-cpp23-req-10.1]

  // Invalid: unrelated classes
  Unrelated* ptr4 = static_cast<Unrelated*>(b1);
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: pointer conversion between unrelated classes 'Base1' and 'Unrelated' should be avoided [automotive-cpp23-req-10.1]

  // Invalid: unrelated classes
  Base1* ptr5 = static_cast<Base1*>(u);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: pointer conversion between unrelated classes 'Unrelated' and 'Base1' should be avoided [automotive-cpp23-req-10.1]

  delete b1;
  delete d1;
  delete b2;
  delete u;
}

void test_reinterpret_cast() {
  Base1* b1 = new Base1();
  Base2* b2 = new Base2();

  // Invalid: reinterpret_cast between unrelated classes
  Base2* ptr1 = reinterpret_cast<Base2*>(b1);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: pointer conversion between unrelated classes 'Base1' and 'Base2' should be avoided [automotive-cpp23-req-10.1]

  // Invalid: reinterpret_cast between unrelated classes
  Base1* ptr2 = reinterpret_cast<Base1*>(b2);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: pointer conversion between unrelated classes 'Base2' and 'Base1' should be avoided [automotive-cpp23-req-10.1]

  delete b1;
  delete b2;
}

// Test with multiple inheritance
class MultiBase1 {
public:
  int a;
};

class MultiBase2 {
public:
  int b;
};

class MultiDerived : public MultiBase1, public MultiBase2 {
public:
  int c;
};

void test_multiple_inheritance() {
  MultiDerived* md = new MultiDerived();
  MultiBase1* mb1 = new MultiBase1();
  MultiBase2* mb2 = new MultiBase2();

  // Valid: derived to base (multiple inheritance)
  MultiBase1* ptr1 = static_cast<MultiBase1*>(md);  // OK
  MultiBase2* ptr2 = static_cast<MultiBase2*>(md);  // OK

  // Valid: base to derived
  MultiDerived* ptr3 = static_cast<MultiDerived*>(mb1);  // OK

  // Invalid: between bases that are unrelated
  MultiBase2* ptr4 = static_cast<MultiBase2*>(mb1);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: pointer conversion between unrelated classes 'MultiBase1' and 'MultiBase2' should be avoided [automotive-cpp23-req-10.1]

  delete md;
  delete mb1;
  delete mb2;
}

// Test with non-class pointer types (should not warn)
void test_non_class_pointers() {
  int* ip = new int(42);
  void* vp = static_cast<void*>(ip);  // OK - not class types
  int* ip2 = static_cast<int*>(vp);   // OK - not class types

  delete ip;
}

// Test same class conversion (should not warn)
void test_same_class() {
  Base1* b1 = new Base1();
  Base1* b2 = static_cast<Base1*>(b1);  // OK - same class

  delete b1;
}

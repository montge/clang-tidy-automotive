// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-15.3 %t

// Test object slicing detection

class Base {
public:
  int x;
  Base() : x(0) {}
  Base(const Base& other) : x(other.x) {}
};

class Derived : public Base {
public:
  int y;
  Derived() : y(0) {}
  Derived(const Derived& other) : Base(other), y(other.y) {}
};

void func_by_value(Base b) {
  // Function takes base by value
}

void func_by_reference(Base& b) {
  // Function takes base by reference
}

void test_slicing() {
  Derived d;

  // Object slicing - should warn
  Base b1 = d;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: object slicing occurs when assigning derived class 'Derived' to base class 'Base' variable [automotive-cpp23-req-15.3]

  // Passing by value causes slicing - should warn
  func_by_value(d);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: object slicing occurs when copying derived class 'Derived' to base class 'Base' [automotive-cpp23-req-15.3]

  // No slicing with reference - should not warn
  func_by_reference(d);

  // No slicing with pointer - should not warn
  Base* ptr = &d;

  // No slicing when copying same type - should not warn
  Derived d2 = d;
  Base b2;
  Base b3 = b2;
}

class AnotherBase {
public:
  int z;
};

void test_no_slicing() {
  // No inheritance relationship - should not warn
  AnotherBase ab;
  Base b;
}

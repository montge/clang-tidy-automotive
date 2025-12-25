// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-11.0.1 %t

// Test object slicing detection for MISRA C++:2023 Rule 11.0.1

class Base {
public:
  int x;
  Base() : x(0) {}
  Base(const Base& other) : x(other.x) {}
  virtual void foo() {}
};

class Derived : public Base {
public:
  int y;
  Derived() : y(0) {}
  Derived(const Derived& other) : Base(other), y(other.y) {}
  void foo() override {}
};

// Function taking base by value
void funcByValue(Base b) {
  // Function takes base by value
}

// Function taking base by reference
void funcByReference(const Base& b) {
  // Function takes base by reference
}

// Function taking base by pointer
void funcByPointer(Base* b) {
  // Function takes base by pointer
}

// Function returning base by value
Base returnByValue(Derived d) {
  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: object slicing occurs when returning derived class 'Derived' as base class 'Base' by value [automotive-cpp23-req-11.0.1]
  return d;
}

// Function returning base by reference
const Base& returnByReference(const Derived& d) {
  return d;  // OK: reference
}

void testSlicing() {
  Derived d;

  // Object slicing via copy construction - should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: object slicing occurs when initializing base class 'Base' variable with derived class 'Derived' [automotive-cpp23-req-11.0.1]
  Base b1(d);

  // Object slicing via assignment - should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: object slicing occurs when initializing base class 'Base' variable with derived class 'Derived' [automotive-cpp23-req-11.0.1]
  Base b2 = d;

  // Passing by value causes slicing - should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: object slicing occurs when copying derived class 'Derived' to base class 'Base' [automotive-cpp23-req-11.0.1]
  funcByValue(d);

  // No slicing with reference - should not warn
  funcByReference(d);

  // No slicing with pointer - should not warn
  funcByPointer(&d);

  // No slicing when copying same type - should not warn
  Derived d2 = d;
  Base b3;
  Base b4 = b3;
}

void testAssignment() {
  Derived d;
  Base b;

  // Assignment operator slicing - should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: object slicing occurs when assigning derived class 'Derived' to base class 'Base' [automotive-cpp23-req-11.0.1]
  b = d;

  // No slicing with same type
  Base b2;
  b = b2;  // OK
}

class AnotherBase {
public:
  int z;
};

void testNoSlicing() {
  // No inheritance relationship - should not warn
  AnotherBase ab;
  Base b;
}

class MultiLevel : public Derived {
public:
  int z;
};

void testMultiLevelInheritance() {
  MultiLevel ml;

  // Multi-level inheritance slicing - should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: object slicing occurs when initializing base class 'Base' variable with derived class 'MultiLevel' [automotive-cpp23-req-11.0.1]
  Base b1 = ml;

  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: object slicing occurs when initializing base class 'Derived' variable with derived class 'MultiLevel' [automotive-cpp23-req-11.0.1]
  Derived d1 = ml;
}

// Template function test
template<typename T>
void templateFunc(T t) {
  Base b = t;  // May or may not slice depending on T
}

void testTemplate() {
  Derived d;
  templateFunc<Derived>(d);  // Instantiation may trigger slicing
}

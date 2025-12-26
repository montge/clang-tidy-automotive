// RUN: %check_clang_tidy %s automotive-cpp23-req-19.2 %t

// MISRA C++:2023 Rule 19.2
// A reference or pointer to a derived class shall not be implicitly cast to a
// reference or pointer to a base class.

class Base {
public:
  int x;
  virtual void foo() {}
  virtual ~Base() = default;
};

class Derived : public Base {
public:
  int y;
  void foo() override {}
};

class AnotherDerived : public Base {
public:
  int z;
  void foo() override {}
};

class Unrelated {
public:
  int w;
};

// Function taking Base pointer
void takeBasePointer(Base* b) {}

// Function taking Base reference
void takeBaseReference(Base& b) {}

// Function returning Base pointer
Base* returnBasePointer(Derived* d) {
  return d; // CHECK-MESSAGES: :[[@LINE]]:10: warning: implicit upcast from derived class pointer Derived to base class pointer Base; use static_cast for explicit conversion
}

// Function returning Base reference
Base& returnBaseReference(Derived& d) {
  return d; // CHECK-MESSAGES: :[[@LINE]]:10: warning: implicit upcast from derived class reference Derived to base class reference Base; use static_cast for explicit conversion
}

void testPointerCasts() {
  Derived d;
  Derived* pd = &d;

  // Implicit upcast of pointer - should warn
  Base* b1 = pd;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: implicit upcast from derived class pointer Derived to base class pointer Base; use static_cast for explicit conversion

  // Implicit upcast via assignment - should warn
  Base* b2;
  b2 = pd;
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: implicit upcast from derived class pointer Derived to base class pointer Base; use static_cast for explicit conversion

  // Implicit upcast in function argument - should warn
  takeBasePointer(pd);
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: implicit upcast from derived class pointer Derived to base class pointer Base; use static_cast for explicit conversion

  // Explicit cast - should not warn (compliant)
  Base* b3 = static_cast<Base*>(pd);
}

void testReferenceCasts() {
  Derived d;

  // Implicit upcast of reference - should warn
  Base& br1 = d;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: implicit upcast from derived class reference Derived to base class reference Base; use static_cast for explicit conversion

  // Implicit upcast via reference binding - should warn
  Base& br2(d);
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: implicit upcast from derived class reference Derived to base class reference Base; use static_cast for explicit conversion

  // Implicit upcast in function argument - should warn
  takeBaseReference(d);
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: implicit upcast from derived class reference Derived to base class reference Base; use static_cast for explicit conversion

  // Explicit cast - should not warn (compliant)
  Base& br3 = static_cast<Base&>(d);
}

void testComplexHierarchy() {
  AnotherDerived ad;
  AnotherDerived* pad = &ad;

  // Implicit upcast from another derived class - should warn
  Base* b = pad;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: implicit upcast from derived class pointer AnotherDerived to base class pointer Base; use static_cast for explicit conversion
}

void testNonRelatedTypes() {
  Unrelated u;
  Unrelated* pu = &u;

  // No warning for unrelated types (not an upcast)
  // Base* b = pu; // This would be a compilation error anyway
}

void testConditionalOperator() {
  Derived d1;
  Derived d2;
  bool condition = true;

  // Implicit upcast in conditional expression - should warn (single warning for expression)
  Base* b = condition ? &d1 : &d2;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: implicit upcast from derived class pointer Derived to base class pointer Base; use static_cast for explicit conversion
}

void testArrayContext() {
  Derived d[3];

  // Implicit upcast of array element pointer - should warn
  Base* b = &d[0];
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: implicit upcast from derived class pointer Derived to base class pointer Base; use static_cast for explicit conversion
}

// Compliant examples
void compliantExamples() {
  Derived d;
  Derived* pd = &d;

  // Explicit static_cast - compliant
  Base* b1 = static_cast<Base*>(pd);
  Base& br1 = static_cast<Base&>(d);

  // Same type assignment - compliant
  Derived* pd2 = pd;
  Derived& dr = d;

  // Explicit static_cast in function calls - compliant
  takeBasePointer(static_cast<Base*>(pd));
  takeBaseReference(static_cast<Base&>(d));
}

// Edge case: const correctness
void testConstCorrectness() {
  const Derived d{};
  const Derived* pd = &d;

  // Implicit upcast with const - should warn
  const Base* b = pd;
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: implicit upcast from derived class pointer Derived to base class pointer Base; use static_cast for explicit conversion

  const Base& br = d;
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: implicit upcast from derived class reference Derived to base class reference Base; use static_cast for explicit conversion
}

// Multiple inheritance
class Interface {
public:
  virtual void bar() = 0;
  virtual ~Interface() = default;
};

class MultiDerived : public Base, public Interface {
public:
  void bar() override {}
};

void testMultipleInheritance() {
  MultiDerived md;
  MultiDerived* pmd = &md;

  // Implicit upcast to first base - should warn
  Base* b = pmd;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: implicit upcast from derived class pointer MultiDerived to base class pointer Base; use static_cast for explicit conversion

  // Implicit upcast to second base - should warn
  Interface* i = pmd;
  // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: implicit upcast from derived class pointer MultiDerived to base class pointer Interface; use static_cast for explicit conversion
}

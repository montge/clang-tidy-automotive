// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-adv-17.3.1 %t
// Test for automotive-cpp23-adv-17.3.1: typeid should not be used with
// a polymorphic type operand that is a dereferenced pointer

#include <typeinfo>

// Polymorphic base class (has virtual functions)
class PolymorphicBase {
public:
  virtual ~PolymorphicBase() = default;
  virtual void foo() {}
};

class PolymorphicDerived : public PolymorphicBase {
public:
  void bar() {}
};

// Non-polymorphic class (no virtual functions)
class NonPolymorphic {
public:
  void baz() {}
};

class AnotherPolymorphic {
public:
  virtual void method() {}
  int member;
};

void test_violations() {
  PolymorphicBase* ptr = new PolymorphicDerived();

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(*ptr);

  PolymorphicDerived* derivedPtr = new PolymorphicDerived();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(*derivedPtr);

  AnotherPolymorphic* anotherPtr = new AnotherPolymorphic();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(*anotherPtr);

  // Access through pointer (arrow operator)
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(anotherPtr->member);

  // Array access (also a form of dereference)
  PolymorphicBase* array[5];
  array[0] = new PolymorphicDerived();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(array[0]->member);

  delete ptr;
  delete derivedPtr;
  delete anotherPtr;
  delete array[0];
}

void test_compliant() {
  // OK: Using reference instead of dereferenced pointer
  PolymorphicBase* ptr = new PolymorphicDerived();
  PolymorphicBase& ref = *ptr;
  typeid(ref);  // OK

  // OK: Using value (not a pointer)
  PolymorphicDerived obj;
  typeid(obj);  // OK

  // OK: Using type name (not an expression)
  typeid(PolymorphicBase);  // OK
  typeid(PolymorphicDerived);  // OK

  // OK: Non-polymorphic type (even though dereferenced)
  NonPolymorphic* nonPolyPtr = new NonPolymorphic();
  typeid(*nonPolyPtr);  // OK - non-polymorphic type

  // OK: Pointer itself (not dereferenced)
  PolymorphicBase* ptr2 = new PolymorphicDerived();
  typeid(ptr2);  // OK - checking pointer type, not dereferencing

  delete ptr;
  delete nonPolyPtr;
  delete ptr2;
}

void test_const_and_volatile() {
  const PolymorphicBase* constPtr = new PolymorphicDerived();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(*constPtr);

  volatile PolymorphicBase* volatilePtr = new PolymorphicDerived();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(*volatilePtr);

  delete constPtr;
  delete volatilePtr;
}

void test_multiple_indirection() {
  PolymorphicBase* ptr = new PolymorphicDerived();
  PolymorphicBase** ptrPtr = &ptr;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: typeid should not be used with a polymorphic type operand that is a dereferenced pointer
  typeid(**ptrPtr);

  delete ptr;
}

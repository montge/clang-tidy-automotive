// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-15.7 %t

// Test virtual destructor requirement (MISRA C++:2023 Rule 15.7)

// ============= Violations: Non-virtual destructor with virtual methods =============

class NonVirtualDestructor {
public:
  virtual void foo();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: class 'NonVirtualDestructor' has virtual functions but destructor is not virtual
  ~NonVirtualDestructor() {}
};

class AbstractWithNonVirtual {
public:
  virtual void bar() = 0;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: class 'AbstractWithNonVirtual' has virtual functions but destructor is not virtual
  ~AbstractWithNonVirtual() {}
};

class MultipleVirtuals {
public:
  virtual void method1();
  virtual void method2();
  virtual void method3();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: class 'MultipleVirtuals' has virtual functions but destructor is not virtual
  ~MultipleVirtuals() {}
};

// ============= Violations: No explicit destructor with virtual methods =============

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: class 'NoExplicitDestructor' has virtual functions but no explicit virtual destructor
class NoExplicitDestructor {
public:
  virtual void foo();
  // Implicit destructor - no virtual destructor
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: class 'AbstractNoDestructor' has virtual functions but no explicit virtual destructor
class AbstractNoDestructor {
public:
  virtual void bar() = 0;
};

// ============= Compliant: Virtual destructor =============

class VirtualDestructor {
public:
  virtual void foo();
  virtual ~VirtualDestructor() {}  // Compliant
};

class VirtualDestructorDefault {
public:
  virtual void bar() = 0;
  virtual ~VirtualDestructorDefault() = default;  // Compliant
};

class VirtualDestructorPure {
public:
  virtual void baz();
  virtual ~VirtualDestructorPure() = 0;  // Compliant (pure virtual destructor)
};

// ============= Compliant: Inherits virtual destructor =============

class GoodBase {
public:
  virtual void foo();
  virtual ~GoodBase() {}
};

class InheritsVirtualDestructor : public GoodBase {
public:
  void foo() override;
  ~InheritsVirtualDestructor() {}  // Compliant - base has virtual destructor
};

class DeepInheritance : public InheritsVirtualDestructor {
public:
  void foo() override;
  // Implicit destructor is OK - inherited from GoodBase
};

// ============= Compliant: No virtual methods =============

class NoVirtualMethods {
public:
  void foo();
  ~NoVirtualMethods() {}  // Compliant - no virtual methods
};

struct PlainStruct {
  int x;
  int y;
  // No virtual methods - should not warn
};

class OnlyNonVirtual {
public:
  void method1();
  void method2();
  ~OnlyNonVirtual() {}  // Compliant - no virtual methods
};

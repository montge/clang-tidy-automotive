// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-12.2.1 %t

// Test MISRA C++:2023 Rule 12.2.1:
// A base class destructor shall be public and virtual, or protected and non-virtual.

// ============= Violations: Public non-virtual destructor =============

class PublicNonVirtualDestructor {
public:
  virtual void foo();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor of class 'PublicNonVirtualDestructor' is public but not virtual; should be public and virtual, or protected and non-virtual
  ~PublicNonVirtualDestructor() {}
};

class AbstractPublicNonVirtual {
public:
  virtual void bar() = 0;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor of class 'AbstractPublicNonVirtual' is public but not virtual; should be public and virtual, or protected and non-virtual
  ~AbstractPublicNonVirtual() {}
};

// ============= Violations: Protected virtual destructor =============

class ProtectedVirtualDestructor {
public:
  virtual void foo();
protected:
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: destructor of class 'ProtectedVirtualDestructor' is protected and virtual; should be public and virtual, or protected and non-virtual
  virtual ~ProtectedVirtualDestructor() {}
};

// ============= Violations: Private destructor =============

class PrivateDestructor {
public:
  virtual void foo();
private:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor of class 'PrivateDestructor' is private; should be public and virtual, or protected and non-virtual
  ~PrivateDestructor() {}
};

// ============= Violations: No explicit destructor =============

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: class 'NoExplicitDestructor' is used polymorphically but has no explicit destructor; destructor should be public and virtual, or protected and non-virtual
class NoExplicitDestructor {
public:
  virtual void foo();
  // Implicit destructor - violation
};

// ============= Violations: Derived class with public non-virtual destructor =============

class GoodBase {
public:
  virtual void foo();
  virtual ~GoodBase() {}
};

class DerivedBad : public GoodBase {
public:
  virtual void bar();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor of class 'DerivedBad' is public but not virtual; should be public and virtual, or protected and non-virtual
  ~DerivedBad() {}
};

// ============= Compliant: Public virtual destructor =============

class PublicVirtualDestructor {
public:
  virtual void foo();
  virtual ~PublicVirtualDestructor() {}  // Compliant
};

class PublicVirtualDefault {
public:
  virtual void bar() = 0;
  virtual ~PublicVirtualDefault() = default;  // Compliant
};

class PublicVirtualPure {
public:
  virtual void baz();
  virtual ~PublicVirtualPure() = 0;  // Compliant
};

// ============= Compliant: Protected non-virtual destructor =============

class ProtectedNonVirtualDestructor {
public:
  virtual void foo();
protected:
  ~ProtectedNonVirtualDestructor() {}  // Compliant
};

class ProtectedNonVirtualDefault {
public:
  virtual void bar() = 0;
protected:
  ~ProtectedNonVirtualDefault() = default;  // Compliant
};

// ============= Compliant: No virtual methods =============

class NoVirtualMethods {
public:
  void foo();
  ~NoVirtualMethods() {}  // Compliant - not polymorphic
};

struct PlainStruct {
  int x;
  int y;
  // No virtual methods - should not warn
};

// ============= Compliant: Inherits public virtual destructor =============

class CompliantBase {
public:
  virtual void foo();
  virtual ~CompliantBase() {}
};

class CompliantDerived : public CompliantBase {
public:
  void foo() override;
  virtual ~CompliantDerived() {}  // Compliant - public and virtual
};

// ============= Compliant: Inherits protected non-virtual destructor =============

class ProtectedBase {
public:
  virtual void foo();
protected:
  ~ProtectedBase() {}  // Protected non-virtual - compliant
};

class DerivedFromProtected : public ProtectedBase {
public:
  void foo() override;
protected:
  ~DerivedFromProtected() {}  // Compliant - protected non-virtual
};

// ============= Edge case: Multiple inheritance =============

class Base1 {
public:
  virtual void foo();
  virtual ~Base1() {}
};

class Base2 {
public:
  virtual void bar();
  virtual ~Base2() {}
};

class MultipleInheritance : public Base1, public Base2 {
public:
  void foo() override;
  void bar() override;
  virtual ~MultipleInheritance() {}  // Compliant
};

// ============= Edge case: Template classes =============

template<typename T>
class TemplateWithVirtual {
public:
  virtual void process(T value);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor of class 'TemplateWithVirtual<T>' is public but not virtual; should be public and virtual, or protected and non-virtual
  ~TemplateWithVirtual() {}
};

template<typename T>
class TemplateCompliant {
public:
  virtual void process(T value);
  virtual ~TemplateCompliant() {}  // Compliant
};

// Instantiate templates to trigger checks
void instantiate_templates() {
  TemplateWithVirtual<int> twv;
  TemplateCompliant<int> tc;
}

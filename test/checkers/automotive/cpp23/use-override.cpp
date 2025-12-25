// RUN: %check_clang_tidy -std=c++11 %s automotive-cpp23-req-13.3.2 %t

// Test override specifier usage in derived classes

class Base {
public:
  virtual void foo();
  virtual void bar();
  virtual void baz();
  virtual int getValue();
};

class Derived : public Base {
public:
  // Missing override - should warn
  void foo();
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: overriding function 'Derived::foo' should be marked with 'override' specifier [automotive-cpp23-req-13.3.2]

  // Has override - OK
  void bar() override;

  // Has final (implies override) - OK
  void baz() final;

  int getValue();
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: overriding function 'Derived::getValue' should be marked with 'override' specifier
};

class AnotherBase {
public:
  virtual void method1();
  virtual void method2();
  virtual void method3();
};

class AnotherDerived : public AnotherBase {
public:
  // Missing override - should warn
  virtual void method1();
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: overriding function 'AnotherDerived::method1' should be marked with 'override' specifier

  // Has override - OK
  virtual void method2() override;

  // Has final - OK
  virtual void method3() final;
};

// Base class - no warning expected
class BaseClass {
public:
  virtual void virtualMethod();
  void nonVirtualMethod();
};

// Multiple inheritance
class Interface1 {
public:
  virtual void interfaceMethod1() = 0;
};

class Interface2 {
public:
  virtual void interfaceMethod2() = 0;
};

class Implementation : public Interface1, public Interface2 {
public:
  // Missing override - should warn
  void interfaceMethod1();
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: overriding function 'Implementation::interfaceMethod1' should be marked with 'override' specifier

  // Has override - OK
  void interfaceMethod2() override;
};

// Const and reference qualifiers
class RefQualBase {
public:
  virtual void constMethod() const;
  virtual void refMethod() &;
};

class RefQualDerived : public RefQualBase {
public:
  // Missing override - should warn
  void constMethod() const;
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: overriding function 'RefQualDerived::constMethod' should be marked with 'override' specifier

  // Has override - OK
  void refMethod() & override;
};

// Deep inheritance hierarchy
class Level1 {
public:
  virtual void hierarchyMethod();
};

class Level2 : public Level1 {
public:
  void hierarchyMethod() override;
};

class Level3 : public Level2 {
public:
  // Missing override - should warn
  void hierarchyMethod();
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: overriding function 'Level3::hierarchyMethod' should be marked with 'override' specifier
};

// Template classes
template <typename T>
class TemplateBase {
public:
  virtual void templateMethod();
};

template <typename T>
class TemplateDerived : public TemplateBase<T> {
public:
  // Missing override - should warn
  void templateMethod();
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: overriding function 'TemplateDerived<int>::templateMethod' should be marked with 'override' specifier
};

// Instantiate template
TemplateDerived<int> instantiated;

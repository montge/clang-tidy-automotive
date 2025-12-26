// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-12.4.1 %t

// Test virtual function default arguments prohibition (MISRA C++:2023 Rule 12.4.1)

// ============= Violations: Virtual functions with default arguments =============

class Base {
public:
  // CHECK-MESSAGES: :[[@LINE+2]]:34: warning: virtual function 'Base::foo' has default argument for parameter 'x'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  virtual void foo(int x = 0);

  // CHECK-MESSAGES: :[[@LINE+2]]:35: warning: virtual function 'Base::bar' has default argument for parameter 'y'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  virtual void bar(int x, int y = 1);

  // CHECK-MESSAGES: :[[@LINE+4]]:33: warning: virtual function 'Base::baz' has default argument for parameter 'a'
  // CHECK-MESSAGES: :[[@LINE+3]]:3: note: virtual function declared here
  // CHECK-MESSAGES: :[[@LINE+2]]:40: warning: virtual function 'Base::baz' has default argument for parameter 'b'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  virtual void baz(int a = 0, int b = 1);

  // Pure virtual with default argument
  // CHECK-MESSAGES: :[[@LINE+2]]:37: warning: virtual function 'Base::pureFunc' has default argument for parameter 'x'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  virtual void pureFunc(int x = 5) = 0;

  virtual ~Base() = default;
};

class Derived : public Base {
public:
  // Override with default argument
  // CHECK-MESSAGES: :[[@LINE+2]]:29: warning: virtual function 'Derived::foo' has default argument for parameter 'x'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  void foo(int x = 1) override;

  // Override with different default than base
  // CHECK-MESSAGES: :[[@LINE+2]]:30: warning: virtual function 'Derived::bar' has default argument for parameter 'y'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  void bar(int x, int y = 2) override;

  // CHECK-MESSAGES: :[[@LINE+2]]:34: warning: virtual function 'Derived::pureFunc' has default argument for parameter 'x'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  void pureFunc(int x = 10) override;
};

class VirtualWithMultipleDefaults {
public:
  // CHECK-MESSAGES: :[[@LINE+4]]:34: warning: virtual function 'VirtualWithMultipleDefaults::func' has default argument for parameter 'a'
  // CHECK-MESSAGES: :[[@LINE+3]]:3: note: virtual function declared here
  // CHECK-MESSAGES: :[[@LINE+2]]:41: warning: virtual function 'VirtualWithMultipleDefaults::func' has default argument for parameter 'b'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  virtual void func(int a = 1, int b = 2, int c = 3);
  // Note: Third parameter should also trigger warning
  // CHECK-MESSAGES: :[[@LINE-2]]:48: warning: virtual function 'VirtualWithMultipleDefaults::func' has default argument for parameter 'c'
  // CHECK-MESSAGES: :[[@LINE-4]]:3: note: virtual function declared here

  virtual ~VirtualWithMultipleDefaults() = default;
};

// ============= Compliant: Virtual functions without default arguments =============

class CompliantBase {
public:
  virtual void foo(int x);           // Compliant - no default
  virtual void bar(int x, int y);    // Compliant - no defaults
  virtual void baz() = 0;            // Compliant - no parameters
  virtual ~CompliantBase() = default;
};

class CompliantDerived : public CompliantBase {
public:
  void foo(int x) override;          // Compliant - no default
  void bar(int x, int y) override;   // Compliant - no defaults
  void baz() override;               // Compliant - no parameters
};

// ============= Compliant: Non-virtual functions with default arguments =============

class NonVirtualWithDefaults {
public:
  void regularFunc(int x = 0);       // Compliant - not virtual
  void anotherFunc(int a = 1, int b = 2); // Compliant - not virtual

  virtual void virtualNoDefaults(int x); // Compliant - virtual but no defaults
};

// ============= Compliant: Virtual function with no parameters =============

class VirtualNoParams {
public:
  virtual void method();             // Compliant - no parameters
  virtual int getValue() = 0;        // Compliant - no parameters
  virtual ~VirtualNoParams() = default;
};

// ============= Edge case: Static methods (not virtual) =============

class StaticMethods {
public:
  static void staticFunc(int x = 0); // Compliant - static cannot be virtual
};

// ============= Edge case: Final methods =============

class FinalBase {
public:
  virtual void method(int x);
  virtual ~FinalBase() = default;
};

class FinalDerived : public FinalBase {
public:
  // CHECK-MESSAGES: :[[@LINE+2]]:32: warning: virtual function 'FinalDerived::method' has default argument for parameter 'x'
  // CHECK-MESSAGES: :[[@LINE+1]]:3: note: virtual function declared here
  void method(int x = 5) final;  // Violation - final is still virtual
};

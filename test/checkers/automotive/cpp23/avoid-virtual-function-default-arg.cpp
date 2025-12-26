// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-12.4.1 %t

// Test virtual function default arguments prohibition (MISRA C++:2023 Rule 12.4.1)

// ============= Violations: Virtual functions with default arguments =============

class Base {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: virtual function 'Base::foo' has default argument for parameter 'x' [automotive-cpp23-req-12.4.1]
  virtual void foo(int x = 0);

  // CHECK-MESSAGES: :[[@LINE+1]]:35: warning: virtual function 'Base::bar' has default argument for parameter 'y' [automotive-cpp23-req-12.4.1]
  virtual void bar(int x, int y = 1);

  // CHECK-MESSAGES: :[[@LINE+2]]:28: warning: virtual function 'Base::baz' has default argument for parameter 'a' [automotive-cpp23-req-12.4.1]
  // CHECK-MESSAGES: :[[@LINE+1]]:39: warning: virtual function 'Base::baz' has default argument for parameter 'b' [automotive-cpp23-req-12.4.1]
  virtual void baz(int a = 0, int b = 1);

  // Pure virtual with default argument
  // CHECK-MESSAGES: :[[@LINE+1]]:33: warning: virtual function 'Base::pureFunc' has default argument for parameter 'x' [automotive-cpp23-req-12.4.1]
  virtual void pureFunc(int x = 5) = 0;

  virtual ~Base() = default;
};

class Derived : public Base {
public:
  // Override with default argument
  // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: virtual function 'Derived::foo' has default argument for parameter 'x' [automotive-cpp23-req-12.4.1]
  void foo(int x = 1) override;

  // Override with different default than base
  // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: virtual function 'Derived::bar' has default argument for parameter 'y' [automotive-cpp23-req-12.4.1]
  void bar(int x, int y = 2) override;

  // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: virtual function 'Derived::pureFunc' has default argument for parameter 'x' [automotive-cpp23-req-12.4.1]
  void pureFunc(int x = 10) override;
};

class VirtualWithMultipleDefaults {
public:
  // CHECK-MESSAGES: :[[@LINE+3]]:29: warning: virtual function 'VirtualWithMultipleDefaults::func' has default argument for parameter 'a' [automotive-cpp23-req-12.4.1]
  // CHECK-MESSAGES: :[[@LINE+2]]:40: warning: virtual function 'VirtualWithMultipleDefaults::func' has default argument for parameter 'b' [automotive-cpp23-req-12.4.1]
  // CHECK-MESSAGES: :[[@LINE+1]]:51: warning: virtual function 'VirtualWithMultipleDefaults::func' has default argument for parameter 'c' [automotive-cpp23-req-12.4.1]
  virtual void func(int a = 1, int b = 2, int c = 3);

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
  // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: virtual function 'FinalDerived::method' has default argument for parameter 'x' [automotive-cpp23-req-12.4.1]
  void method(int x = 5) final;  // Violation - final is still virtual
};

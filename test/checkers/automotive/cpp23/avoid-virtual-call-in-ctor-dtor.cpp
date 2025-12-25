// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-17.8 %t

// Test virtual function call in constructor/destructor (MISRA C++:2023 Rule 17.8)

// ============= Violations: Virtual functions called in constructors =============

class Base {
public:
  Base() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'Base::init' called in constructor 'Base'
    init();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:16: note: virtual function declared here
  virtual void init();

  virtual ~Base() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'Base::cleanup' called in destructor 'Base'
    cleanup();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:16: note: virtual function declared here
  virtual void cleanup();
};

class Derived : public Base {
public:
  Derived() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'Derived::setup' called in constructor 'Derived'
    setup();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:16: note: virtual function declared here
  virtual void setup();

  ~Derived() override {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'Derived::teardown' called in destructor 'Derived'
    teardown();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:16: note: virtual function declared here
  virtual void teardown();
};

// ============= Violations: Explicit this-> calls =============

class ExplicitThis {
public:
  ExplicitThis() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'ExplicitThis::virtualMethod' called in constructor 'ExplicitThis'
    this->virtualMethod();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:16: note: virtual function declared here
  virtual void virtualMethod();

  virtual ~ExplicitThis() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'ExplicitThis::virtualMethod' called in destructor 'ExplicitThis'
    this->virtualMethod();
    // Note points to line 53 above
  }
};

// ============= Violations: Pure virtual function calls =============

class AbstractBase {
public:
  AbstractBase() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'AbstractBase::pureVirtual' called in constructor 'AbstractBase'
    pureVirtual();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:16: note: virtual function declared here
  virtual void pureVirtual() = 0;

  virtual ~AbstractBase() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'AbstractBase::pureVirtual' called in destructor 'AbstractBase'
    pureVirtual();
    // Note points to line 72 above
  }
};

// ============= Violations: Overridden virtual function calls =============

class OverrideBase {
public:
  virtual void doWork();
};

class OverrideDerived : public OverrideBase {
public:
  OverrideDerived() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'OverrideDerived::doWork' called in constructor 'OverrideDerived'
    doWork();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:8: note: virtual function declared here
  void doWork() override;

  ~OverrideDerived() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'OverrideDerived::doWork' called in destructor 'OverrideDerived'
    doWork();
    // Note points to line 96 above
  }
};

// ============= Compliant: Non-virtual function calls =============

class CompliantNonVirtual {
public:
  CompliantNonVirtual() {
    init();  // OK: non-virtual
  }

  void init();  // Not virtual

  ~CompliantNonVirtual() {
    cleanup();  // OK: non-virtual
  }

  void cleanup();  // Not virtual
};

// ============= Compliant: Calls to other objects =============

class Helper {
public:
  virtual void help();
};

class CompliantOtherObject {
public:
  CompliantOtherObject(Helper *h) {
    h->help();  // OK: calling virtual function on another object
  }

  ~CompliantOtherObject() {
    // OK: empty destructor
  }
};

// ============= Compliant: Static functions =============

class CompliantStatic {
public:
  CompliantStatic() {
    staticInit();  // OK: static functions cannot be virtual
  }

  static void staticInit();

  ~CompliantStatic() {
    staticCleanup();  // OK: static functions cannot be virtual
  }

  static void staticCleanup();
};

// ============= Violations: Final methods (still virtual, should warn) =============

class FinalBase {
public:
  virtual void method();
};

class FinalDerived : public FinalBase {
public:
  FinalDerived() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'FinalDerived::method' called in constructor 'FinalDerived'
    method();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:8: note: virtual function declared here
  void method() final;  // Final is still virtual
};

// ============= Violations: Member initialization list (also during construction) =============

class MemberInitList {
public:
  int value;

  // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: virtual function 'MemberInitList::getValue' called in constructor 'MemberInitList'
  MemberInitList() : value(getValue()) {
    // Empty body - virtual call in init list is also a violation
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:15: note: virtual function declared here
  virtual int getValue() { return 0; }
};

// ============= Edge case: Indirect calls through helper =============

class IndirectBase {
public:
  void helper() {
    virtualFunc();  // This is an indirect call
  }

  IndirectBase() {
    helper();  // Calls non-virtual helper, which calls virtual function
              // This is harder to detect statically - not flagged by this check
  }

  virtual void virtualFunc();
};

// ============= Edge case: Constructor delegation (C++11) =============

class DelegatingCtor {
public:
  DelegatingCtor() : DelegatingCtor(0) {
  }

  DelegatingCtor(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: virtual function 'DelegatingCtor::init' called in constructor 'DelegatingCtor'
    init();
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:16: note: virtual function declared here
  virtual void init();
};

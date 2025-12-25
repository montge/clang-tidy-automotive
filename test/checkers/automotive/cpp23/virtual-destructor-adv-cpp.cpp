// RUN: %check_clang_tidy %s automotive-cpp23-adv-12.2.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-12.2.1: virtual base shall have virtual destructor

class Base {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor of 'Base' is not virtual
  ~Base() {}
  virtual void foo() {}
};

class DerivedOK {
public:
  virtual ~DerivedOK() = default;
  virtual void bar() {}
};

class Child : public Base {};

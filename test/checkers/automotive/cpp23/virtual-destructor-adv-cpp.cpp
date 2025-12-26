// RUN: %check_clang_tidy %s automotive-cpp23-adv-12.2.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-12.2.1: virtual base shall have virtual destructor

class Base {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor of class 'Base' is public but not virtual; should be public and virtual, or protected and non-virtual [automotive-cpp23-adv-12.2.1]
  ~Base() {}
  virtual void foo() {}
};

class DerivedOK {
public:
  virtual ~DerivedOK() = default;
  virtual void bar() {}
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: class 'Child' is used polymorphically but has no explicit destructor; destructor should be public and virtual, or protected and non-virtual [automotive-cpp23-adv-12.2.1]
class Child : public Base {};

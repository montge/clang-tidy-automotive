// RUN: %check_clang_tidy %s automotive-cpp23-adv-8.2.5 %t
// Test for automotive-cpp23-adv-8.2.5: dynamic_cast should not be used

class Base {
public:
  virtual ~Base() = default;
  virtual void foo() {}
};

class Derived : public Base {
public:
  void bar() {}
};

void test_dynamic_cast() {
  Base* base = new Derived();

  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: dynamic_cast should not be used
  Derived* d = dynamic_cast<Derived*>(base);
  if (d) {
    d->bar();
  }

  Base& baseRef = *base;
  // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: dynamic_cast should not be used
  Derived& dRef = dynamic_cast<Derived&>(baseRef);
  dRef.bar();

  delete base;
}

void test_compliant() {
  // Using static_cast when type is known
  Derived derived;
  Base* base = &derived;
  Derived* d = static_cast<Derived*>(base);  // OK when type is guaranteed
  d->bar();
}

// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-adv-15.2 %t

// Test virtual base class detection

class Base {
public:
  int x;
  Base() : x(0) {}
};

class AnotherBase {
public:
  int y;
  AnotherBase() : y(0) {}
};

// Violation: virtual inheritance from single base
class Derived1 : virtual public Base {
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: virtual inheritance from 'Base' should be avoided unless necessary [automotive-cpp23-adv-15.2]
public:
  int z;
};

// Violation: virtual inheritance from multiple bases
class Derived2 : virtual public Base, virtual public AnotherBase {
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: virtual inheritance from 'Base' should be avoided unless necessary [automotive-cpp23-adv-15.2]
// CHECK-MESSAGES: :[[@LINE-2]]:43: warning: virtual inheritance from 'AnotherBase' should be avoided unless necessary [automotive-cpp23-adv-15.2]
public:
  int w;
};

// Violation: virtual inheritance with protected access
class Derived3 : virtual protected Base {
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: virtual inheritance from 'Base' should be avoided unless necessary [automotive-cpp23-adv-15.2]
public:
  int a;
};

// Violation: virtual inheritance with private access
class Derived4 : virtual private Base {
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: virtual inheritance from 'Base' should be avoided unless necessary [automotive-cpp23-adv-15.2]
public:
  int b;
};

// Violation: virtual inheritance in diamond pattern
class Middle1 : virtual public Base {
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: virtual inheritance from 'Base' should be avoided unless necessary [automotive-cpp23-adv-15.2]
public:
  int m1;
};

class Middle2 : virtual public Base {
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: virtual inheritance from 'Base' should be avoided unless necessary [automotive-cpp23-adv-15.2]
public:
  int m2;
};

class Diamond : public Middle1, public Middle2 {
public:
  int d;
};

// Compliant: normal (non-virtual) inheritance
class CompliantDerived : public Base {
public:
  int c;
};

// Compliant: multiple non-virtual inheritance
class CompliantMultiple : public Base, public AnotherBase {
public:
  int cm;
};

// Compliant: protected non-virtual inheritance
class CompliantProtected : protected Base {
public:
  int cp;
};

// Compliant: private non-virtual inheritance
class CompliantPrivate : private Base {
public:
  int cpr;
};

// Compliant: no base classes
class Standalone {
public:
  int s;
};

// Mixed case: one virtual, one non-virtual (both should be detected separately)
class MixedInheritance : virtual public Base, public AnotherBase {
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: virtual inheritance from 'Base' should be avoided unless necessary [automotive-cpp23-adv-15.2]
public:
  int mix;
};

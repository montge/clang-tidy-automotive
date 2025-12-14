// RUN: %check_clang_tidy %s automotive-cpp23-req-15.1.3 %t

// Test: Single-argument constructors should be marked explicit

class Implicit {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: single-argument constructor should be marked explicit
  Implicit(int x);

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: single-argument constructor should be marked explicit
  Implicit(double y);
};

class ImplicitWithDefault {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: single-argument constructor should be marked explicit
  ImplicitWithDefault(int x, int y = 0);

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: single-argument constructor should be marked explicit
  ImplicitWithDefault(double x, double y = 0.0, double z = 0.0);
};

class Explicit {
  // OK: marked explicit
  explicit Explicit(int x);

  // OK: marked explicit
  explicit Explicit(double y);
};

class ExplicitWithDefault {
  // OK: marked explicit
  explicit ExplicitWithDefault(int x, int y = 0);
};

class MultipleRequired {
  // OK: requires multiple arguments
  MultipleRequired(int x, int y);

  // OK: requires multiple arguments
  MultipleRequired(int x, int y, int z);
};

class DefaultConstructor {
  // OK: default constructor
  DefaultConstructor();
};

class CopyMove {
  // OK: copy constructor
  CopyMove(const CopyMove& other);

  // OK: move constructor
  CopyMove(CopyMove&& other);
};

// Edge case: constructor with same type parameter (conversion from derived)
class Base {};
class Derived : public Base {
  // This would be a converting constructor, should warn
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: single-argument constructor should be marked explicit
  Derived(int x);
};

// RUN: %check_clang_tidy %s automotive-cpp23-req-8.1.1 %t -- -- -std=c++14 -Wno-c++14-extensions
// Test for automotive-cpp23-req-8.1.1
// Related MISRA C++:2023 Rule: 8.1.1

// This test verifies that implicit 'this' captures in lambdas are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

class TestClass {
  int member = 42;

  void test_implicit_this_capture_by_copy() {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: lambda implicitly captures 'this' via [=]; use explicit [this] or [*this] capture instead
    auto f = [=]() { return member; };
    (void)f;
  }

  void test_implicit_this_capture_by_ref() {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: lambda implicitly captures 'this' via [&]; use explicit [this] capture instead
    auto f = [&]() { return member; };
    (void)f;
  }

  void test_implicit_this_with_explicit_var_copy() {
    int x = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: lambda implicitly captures 'this' via [=]; use explicit [this] or [*this] capture instead
    auto f = [=, &x]() { return member + x; };
    (void)f;
  }

  void test_implicit_this_with_explicit_var_ref() {
    int x = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: lambda implicitly captures 'this' via [&]; use explicit [this] capture instead
    auto f = [&, x]() { return member + x; };
    (void)f;
  }

  void test_method_call_implicit_this() {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: lambda implicitly captures 'this' via [=]; use explicit [this] or [*this] capture instead
    auto f = [=]() { return getMember(); };
    (void)f;
  }

  int getMember() const { return member; }
};

struct NestedCapture {
  int value = 10;

  void outer() {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: lambda implicitly captures 'this' via [=]; use explicit [this] or [*this] capture instead
    auto f = [=]() {
      return value;
    };
    (void)f;
  }
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

class CompliantClass {
  int member = 42;

  void test_explicit_this_capture() {
    // Explicit 'this' capture - no warning
    auto f = [this]() { return member; };
    (void)f;
  }

  void test_explicit_this_with_var() {
    int x = 1;
    // Explicit 'this' and explicit variable - no warning
    auto f = [this, x]() { return member + x; };
    (void)f;
  }

  void test_no_capture() {
    // Lambda doesn't capture anything - no warning
    auto f = []() { return 42; };
    (void)f;
  }

  void test_no_this_capture() {
    int x = 1, y = 2;
    // Lambda uses default capture but doesn't capture 'this' - no warning
    auto f = [=]() { return x + y; };
    (void)f;
  }

  void test_explicit_variable_only() {
    int x = 1;
    // Explicit variable capture, no 'this' - no warning
    auto f = [x]() { return x; };
    (void)f;
  }

  int getMember() const { return member; }
};

// C++17 feature: explicit copy of 'this' via [*this]
// This test will only run when compiled with C++17 or later
#if __cplusplus >= 201703L
class Cpp17Class {
  int member = 42;

  void test_explicit_this_copy() {
    // Explicit copy of 'this' - no warning
    auto f = [*this]() { return member; };
    (void)f;
  }

  void test_explicit_this_copy_with_var() {
    int x = 1;
    // Explicit copy of 'this' and explicit variable - no warning
    auto f = [*this, x]() { return member + x; };
    (void)f;
  }
};
#endif

// Free function - no 'this' to capture
void free_function() {
  int x = 1, y = 2;
  // Lambda in free function with default capture - no 'this' to capture
  auto f = [=]() { return x + y; };
  (void)f;
}

// Static member function - no 'this' to capture
class StaticMemberClass {
  static void static_function() {
    int x = 1, y = 2;
    // Lambda in static member function - no 'this' to capture
    auto f = [=]() { return x + y; };
    (void)f;
  }
};

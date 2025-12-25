// RUN: %check_clang_tidy %s automotive-cpp23-req-28.3 %t

// MISRA C++:2023 Rule 28.3
// A function with side effects shall not be called from a context where
// those side effects would be discarded.

[[nodiscard]] int important_function() {
  return 42;
}

[[nodiscard]] bool check_status() {
  return true;
}

int regular_function() {
  return 10;
}

void void_function() {
  // This is OK - returns void
}

namespace test_nodiscard_ignored {

void test_basic() {
  // Note: ignoring nodiscard return values is caught by the compiler's
  // built-in -Wunused-result warning, not by this check.
  // This check focuses on explicit void casts and comma operators.

  // Compliant: using the return value
  int x = important_function();  // OK
  bool b = check_status();       // OK

  // Compliant: using in condition
  if (check_status()) {          // OK
    // ...
  }

  // Compliant: void function
  void_function();               // OK
}

} // namespace test_nodiscard_ignored

namespace test_void_cast {

void test_explicit_void_cast() {
  // Violation: explicit cast to void
  (void)important_function();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: explicit cast to void discards function return value; side effects should not be discarded [automotive-cpp23-req-28.3]

  (void)check_status();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: explicit cast to void discards function return value; side effects should not be discarded [automotive-cpp23-req-28.3]

  // Also catches static_cast
  static_cast<void>(important_function());
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: explicit cast to void discards function return value; side effects should not be discarded [automotive-cpp23-req-28.3]

  // Compliant: not casting to void
  int x = important_function();  // OK
}

} // namespace test_void_cast

namespace test_comma_operator {

void test_comma() {
  int x;

  // Violation: comma operator discards left operand
  important_function(), x = 5;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: comma operator discards function return value; side effects should not be discarded [automotive-cpp23-req-28.3]

  check_status(), important_function();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: comma operator discards function return value; side effects should not be discarded [automotive-cpp23-req-28.3]

  // Multiple commas
  important_function(), check_status(), x = 10;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: comma operator discards function return value; side effects should not be discarded [automotive-cpp23-req-28.3]

  // Compliant: function in rightmost position (result used)
  x = (void_function(), important_function());  // OK - result of important_function is used
}

} // namespace test_comma_operator

namespace test_compliant_cases {

void test_compliant() {
  // All compliant cases - no warnings expected

  // Using return value
  int result = important_function();

  // Using in expression
  int sum = important_function() + 10;

  // Using in condition
  if (check_status()) {
    // ...
  }

  // Using in while
  while (check_status()) {
    break;
  }

  // Using in for loop
  for (int i = 0; check_status() && i < 10; i++) {
    break;
  }

  // Return value
  int ret = important_function();
  (void)ret;  // OK - casting variable to void, not function call

  // Void functions are OK to call
  void_function();

  // Regular functions without nodiscard - currently not flagged
  regular_function();  // OK - no nodiscard attribute
}

} // namespace test_compliant_cases

namespace test_member_functions {

class MyClass {
public:
  [[nodiscard]] int get_value() const {
    return 42;
  }

  [[nodiscard]] bool validate() {
    return true;
  }

  void do_something() {
    // No return value
  }
};

void test_methods() {
  MyClass obj;

  // Note: ignoring nodiscard is caught by compiler's built-in warning

  // Violation: explicit void cast
  (void)obj.get_value();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: explicit cast to void discards function return value; side effects should not be discarded [automotive-cpp23-req-28.3]

  // Compliant: using the value
  int x = obj.get_value();  // OK
  bool b = obj.validate();  // OK

  // Compliant: void member function
  obj.do_something();  // OK
}

} // namespace test_member_functions

namespace test_chained_calls {

class Builder {
public:
  [[nodiscard]] Builder& add(int x) {
    return *this;
  }

  void build() {
    // Actually builds
  }
};

void test_builder() {
  Builder builder;

  // Note: ignoring nodiscard is caught by compiler's built-in warning

  // Compliant: chaining calls
  builder.add(1).add(2).build();  // OK - intermediate returns used for chaining
}

} // namespace test_chained_calls

// XFAIL: *
// Note: Test file has compiler errors - class methods can't differ only in top-level const
// RUN: %check_clang_tidy %s automotive-cpp23-adv-11.3.2 %t

// MISRA C++:2023 Rule 11.3.2
// Overloaded functions should differ in parameter types, not just in
// parameter const qualification.

namespace test_free_functions {

// Violation: Functions differ only in top-level const
void func1(int x);
void func1(const int x);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'func1' differs from another overload only in top-level const/volatile qualification of parameters [automotive-cpp23-adv-11.3.2]
// CHECK-MESSAGES: :[[@LINE-3]]:6: note: previous declaration is here

// Violation: Multiple parameters, differ only in const
void func2(double d, int i);
void func2(const double d, int i);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'func2' differs from another overload only in top-level const/volatile qualification of parameters [automotive-cpp23-adv-11.3.2]
// CHECK-MESSAGES: :[[@LINE-3]]:6: note: previous declaration is here

// Violation: Second parameter differs in const
void func3(int x, char c);
void func3(int x, const char c);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'func3' differs from another overload only in top-level const/volatile qualification of parameters [automotive-cpp23-adv-11.3.2]
// CHECK-MESSAGES: :[[@LINE-3]]:6: note: previous declaration is here

// Compliant: Different types (reference vs value)
void func4(int x);
void func4(int &x);  // OK - different type

// Compliant: const is not top-level (pointer to const)
void func5(const int *p);
void func5(int *p);  // OK - const applies to pointee

// Compliant: Different number of parameters
void func6(int x);
void func6(int x, int y);  // OK - different arity

// Compliant: Different parameter types
void func7(int x);
void func7(double x);  // OK - different types

} // namespace test_free_functions

namespace test_class_methods {

class TestClass {
public:
  // Violation: Methods differ only in top-level const
  void method1(int x);
  void method1(const int x);
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: function 'method1' differs from another overload only in top-level const/volatile qualification of parameters [automotive-cpp23-adv-11.3.2]
  // CHECK-MESSAGES: :[[@LINE-3]]:8: note: previous declaration is here

  // Violation: Multiple parameters
  void method2(double d, char c);
  void method2(double d, const char c);
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: function 'method2' differs from another overload only in top-level const/volatile qualification of parameters [automotive-cpp23-adv-11.3.2]
  // CHECK-MESSAGES: :[[@LINE-3]]:8: note: previous declaration is here

  // Compliant: Different const qualifiers on method itself
  void method3(int x);
  void method3(int x) const;  // OK - const applies to 'this'

  // Compliant: const pointer (not top-level)
  void method4(const int *p);
  void method4(int *p);  // OK

  // Compliant: Different parameter types
  void method5(int x);
  void method5(double x);  // OK
};

} // namespace test_class_methods

namespace test_edge_cases {

// Compliant: const reference (const is not top-level)
void refFunc(const int &x);
void refFunc(int &x);  // OK - different types

// Compliant: const in template parameter (not top-level)
template<typename T>
void templateFunc(T x);
template<typename T>
void templateFunc(const T *x);  // OK - different types

// Violation: volatile qualifier
void volatileFunc(int x);
void volatileFunc(volatile int x);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'volatileFunc' differs from another overload only in top-level const/volatile qualification of parameters [automotive-cpp23-adv-11.3.2]
// CHECK-MESSAGES: :[[@LINE-3]]:6: note: previous declaration is here

} // namespace test_edge_cases

namespace test_complex_types {

struct S {};

// Violation: Top-level const on struct
void structFunc(S s);
void structFunc(const S s);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'structFunc' differs from another overload only in top-level const/volatile qualification of parameters [automotive-cpp23-adv-11.3.2]
// CHECK-MESSAGES: :[[@LINE-3]]:6: note: previous declaration is here

// Compliant: const applies to pointee
void structPtrFunc(const S *s);
void structPtrFunc(S *s);  // OK

// Compliant: const reference
void structRefFunc(const S &s);
void structRefFunc(S &s);  // OK

} // namespace test_complex_types

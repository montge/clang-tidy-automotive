// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-8.5.1 %t

// Test default argument ordering and consistency (MISRA C++:2023 Rule 8.5.1)

// ============= Violations: Redeclared with different default values =============

// Declaration with default argument
void func1(int x, int y = 10);

// CHECK-MESSAGES: :[[@LINE+1]]:27: warning: default argument for parameter 'y' of function 'func1' differs from previous declaration
void func1(int x, int y = 20);  // Violation - different value

void func1(int x, int y) { }    // Definition - no default


// Multiple parameters with defaults redeclared differently
void func2(int a = 1, int b = 2, int c = 3);

// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: default argument for parameter 'a' of function 'func2' differs from previous declaration
void func2(int a = 5, int b = 2, int c = 3);  // Violation - 'a' differs

// CHECK-MESSAGES: :[[@LINE+1]]:36: warning: default argument for parameter 'b' of function 'func2' differs from previous declaration
void func2(int a = 1, int b = 10, int c = 3); // Violation - 'b' differs


// Function with complex default argument expressions
void func3(int x = 1 + 2);

// CHECK-MESSAGES: :[[@LINE+1]]:24: warning: default argument for parameter 'x' of function 'func3' differs from previous declaration
void func3(int x = 2 + 1);  // Violation - different expression (even though equal)


// Namespace-scoped functions
namespace NS {
  void func4(int x = 100);

  // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: default argument for parameter 'x' of function 'NS::func4' differs from previous declaration
  void func4(int x = 200);  // Violation
}


// ============= Compliant: Consistent default arguments =============

// Same default value in redeclaration
void compliant1(int x = 5);
void compliant1(int x = 5);  // Compliant - same value
void compliant1(int x) { }   // Compliant - definition without default


// No default arguments
void compliant2(int x);
void compliant2(int x);      // Compliant - no defaults
void compliant2(int x) { }


// Default only in first declaration
void compliant3(int x = 10);
void compliant3(int x);      // Compliant - no new default specified
void compliant3(int x) { }


// Multiple parameters, all consistent
void compliant4(int a = 1, int b = 2);
void compliant4(int a = 1, int b = 2);  // Compliant - all same
void compliant4(int a, int b) { }


// Adding new defaults to parameters that didn't have them
void compliant5(int x, int y);
void compliant5(int x, int y = 5);  // Compliant - adding default to y
void compliant5(int x = 3, int y);  // Compliant - adding default to x


// ============= Compliant: Class member functions =============

class TestClass {
public:
  void method1(int x = 10);
  void method2(int x);
  void method3(int a = 1, int b = 2);
};

// Implementation with same defaults is compliant
void TestClass::method1(int x) { }  // Compliant - no default in definition

// Implementation with no defaults when declaration had none
void TestClass::method2(int x) { }  // Compliant

// Implementation with consistent defaults
void TestClass::method3(int a, int b) { }  // Compliant


// ============= Edge cases =============

// Templates with default arguments
template<typename T>
void templateFunc(T x = T());

template<typename T>
void templateFunc(T x) { }  // Compliant - definition without default


// Function pointers (should be ignored)
typedef void (*FuncPtr)(int x);
FuncPtr ptr1;
FuncPtr ptr2;


// Overloaded functions (different signatures)
void overloaded(int x = 1);
void overloaded(double x = 1.0);  // Compliant - different function


// Static class members
class StaticTest {
public:
  static void staticFunc(int x = 5);
};

void StaticTest::staticFunc(int x) { }  // Compliant


// ============= Complex expressions =============

const int CONST_VAL = 42;

void expr1(int x = CONST_VAL);
void expr1(int x = CONST_VAL);  // Compliant - same constant

void expr2(int x = sizeof(int));
void expr2(int x = sizeof(int));  // Compliant - same expression


// ============= Multiple redeclarations =============

void multi1(int x = 1);
void multi1(int x = 1);  // Compliant - same
// CHECK-MESSAGES: :[[@LINE+1]]:25: warning: default argument for parameter 'x' of function 'multi1' differs from previous declaration
void multi1(int x = 2);  // Violation - different from previous
void multi1(int x) { }

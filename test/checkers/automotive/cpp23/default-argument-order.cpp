// RUN: %check_clang_tidy %s automotive-cpp23-req-8.5.1 %t

// Test default argument ordering and consistency (MISRA C++:2023 Rule 8.5.1)
// All test cases are compliant - no warnings expected.
// Note: Redefining default arguments with different values is a C++ compiler error,
// so this test only covers compliant patterns (default in first declaration only).

// ============= Compliant: Default only in first declaration =============

// Declaration with default argument
void func1(int x, int y = 10);
void func1(int x, int y);       // OK - subsequent declaration without default
void func1(int x, int y) { }    // Definition - no default

// Multiple parameters with defaults
void func2(int a = 1, int b = 2, int c = 3);
void func2(int a, int b, int c) { }  // OK - definition without defaults

// ============= Compliant: Class member functions =============

class TestClass {
public:
  void method1(int x = 10);
  void method2(int x);
  void method3(int a = 1, int b = 2);
};

// Implementation without defaults is compliant
void TestClass::method1(int x) { }
void TestClass::method2(int x) { }
void TestClass::method3(int a, int b) { }

// ============= Compliant: Templates =============

template<typename T>
void templateFunc(T x = T());

template<typename T>
void templateFunc(T x) { }  // Compliant - definition without default

// ============= Compliant: Overloaded functions =============

void overloaded(int x = 1);
void overloaded(double x = 1.0);  // Different function signature

void overloaded(int x) { }
void overloaded(double x) { }

// ============= Compliant: Static class members =============

class StaticTest {
public:
  static void staticFunc(int x = 5);
};

void StaticTest::staticFunc(int x) { }

// ============= Compliant: Adding defaults in later declarations =============

void addDefaults(int x, int y);
void addDefaults(int x, int y = 5);     // OK - adding default to y
void addDefaults(int x = 3, int y);     // OK - adding default to x
void addDefaults(int x, int y) { }

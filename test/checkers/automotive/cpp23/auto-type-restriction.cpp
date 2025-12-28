// RUN: %check_clang_tidy %s automotive-cpp23-adv-10.1.2 %t

// MISRA C++:2023 Rule 10.1.2 - auto type specifier restrictions
// The auto type specifier shall only be used when the type is explicitly
// stated or obvious from context.

class MyClass {
public:
  MyClass() = default;
  int getValue() const { return 42; }
};

int getInteger() { return 42; }
double getDouble() { return 3.14; }
MyClass getObject() { return MyClass(); }

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void testCompliantCases() {
  // Compliant: Explicit cast - type is obvious
  auto x1 = static_cast<int>(42);
  auto x5 = (int)42; // C-style cast

  // Compliant: new expression - type is explicit
  auto ptr1 = new int;
  auto ptr2 = new MyClass();
  auto ptr3 = new int[10];

  // Cleanup
  delete ptr1;
  delete ptr2;
  delete[] ptr3;

  (void)x1; (void)x5;
}

//===----------------------------------------------------------------------===//
// Non-Compliant Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void testNonCompliantCases() {
  // Non-compliant: Function return type not obvious
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer; consider using an explicit type or a cast to make the type clear [automotive-cpp23-adv-10.1.2]
  auto y1 = getInteger();

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y2 = getDouble();

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y3 = getObject();

  MyClass obj;
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y4 = obj.getValue();

  // Non-compliant: Literal values - ambiguous type
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y5 = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y6 = 3.14;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y7 = true;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y8 = 'c';

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y9 = "string";

  // Non-compliant: Complex expressions
  int a = 10, b = 20;
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y10 = a + b;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y11 = a * b + 5;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y12 = a > b ? a : b;

  // Non-compliant: Array access
  int arr[10];
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y13 = arr[0];

  // Non-compliant: Pointer dereference
  int* ptr = nullptr;
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto y14 = *ptr;

  (void)y1; (void)y2; (void)y3; (void)y4; (void)y5;
  (void)y6; (void)y7; (void)y8; (void)y9; (void)y10;
  (void)y11; (void)y12; (void)y13; (void)y14;
}

// Edge cases
void testEdgeCases() {
  // Compliant: Cast expression
  auto x = static_cast<double>(static_cast<int>(3.14));

  // Compliant: Cast wrapping function call
  auto y = static_cast<int>(getDouble());

  // Non-compliant: Assignment from another auto variable
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto a = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto b = a;

  (void)x; (void)y; (void)b;
}

// Template function edge case
template<typename T>
T getValue() { return T(); }

void testTemplates() {
  // Non-compliant: Template function result
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: use of 'auto' type specifier where type is not obvious from initializer
  auto x = getValue<int>();
  (void)x;
}

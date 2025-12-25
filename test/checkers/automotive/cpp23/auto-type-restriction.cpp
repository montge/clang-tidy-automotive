// RUN: %check_clang_tidy %s automotive-cpp23-adv-10.1.2 %t

// MISRA C++:2023 Rule 10.1.2 - auto type specifier restrictions
// The auto type specifier shall only be used when the type is explicitly
// stated or obvious from context.

#include <memory>
#include <vector>
#include <utility>

class MyClass {
public:
  MyClass() = default;
  int getValue() const { return 42; }
};

int getInteger() { return 42; }
double getDouble() { return 3.14; }
MyClass getObject() { return MyClass(); }

void testCompliantCases() {
  // Compliant: Explicit cast - type is obvious
  auto x1 = static_cast<int>(42);
  auto x2 = reinterpret_cast<int*>(nullptr);
  auto x3 = const_cast<int*>(nullptr);
  auto x4 = dynamic_cast<MyClass*>(nullptr);
  auto x5 = (int)42; // C-style cast

  // Compliant: new expression - type is explicit
  auto ptr1 = new int;
  auto ptr2 = new MyClass();
  auto ptr3 = new int[10];

  // Compliant: make_unique, make_shared - type in template arg
  auto smart1 = std::make_unique<int>(42);
  auto smart2 = std::make_shared<MyClass>();
  auto smart3 = std::make_unique<MyClass[]>(10);

  // Compliant: Lambda expressions - auto is appropriate/required
  auto lambda1 = []() { return 42; };
  auto lambda2 = [](int x) { return x * 2; };
  auto lambda3 = [&](int x) -> double { return x * 3.14; };

  // Compliant: Explicit type construction
  auto obj1 = MyClass();
  auto obj2 = std::vector<int>();
  auto obj3 = std::vector<int>{1, 2, 3};
  auto val1 = int(42);
  auto val2 = double(3.14);

  // Compliant: Range-based for loops
  std::vector<int> vec = {1, 2, 3, 4, 5};
  for (auto& item : vec) {
    (void)item;
  }
  for (auto&& item : vec) {
    (void)item;
  }
  for (const auto& item : vec) {
    (void)item;
  }

  // Compliant: make_pair
  auto pair = std::make_pair(1, 2);
}

void testNonCompliantCases() {
  // Non-compliant: Function return type not obvious
  auto y1 = getInteger();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer; consider using an explicit type or a cast to make the type clear

  auto y2 = getDouble();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto y3 = getObject();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  MyClass obj;
  auto y4 = obj.getValue();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  // Non-compliant: Literal values - ambiguous type
  auto y5 = 42;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto y6 = 3.14;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto y7 = true;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto y8 = 'c';
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto y9 = "string";
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  // Non-compliant: Complex expressions
  int a = 10, b = 20;
  auto y10 = a + b;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto y11 = a * b + 5;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto y12 = a > b ? a : b;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  // Non-compliant: Array access
  int arr[10];
  auto y13 = arr[0];
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  // Non-compliant: Pointer dereference
  int* ptr = nullptr;
  auto y14 = *ptr;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer
}

// Edge cases
void testEdgeCases() {
  // Compliant: Nested casts
  auto x = static_cast<double>(static_cast<int>(3.14));

  // Compliant: Cast in expression
  auto y = static_cast<int>(getDouble());

  // Non-compliant: Assignment from another auto variable
  auto a = 42;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer

  auto b = a;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer
}

// Template function edge case
template<typename T>
T getValue() { return T(); }

void testTemplates() {
  // Non-compliant: Template function result
  auto x = getValue<int>();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: use of 'auto' type specifier where type is not obvious from initializer
}

// C++17 structured bindings - should be compliant (auto is required)
#if __cplusplus >= 201703L
void testStructuredBindings() {
  std::pair<int, double> p{1, 2.0};
  auto [x, y] = p; // Compliant - structured binding requires auto
}
#endif

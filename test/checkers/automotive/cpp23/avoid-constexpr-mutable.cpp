// RUN: %check_clang_tidy %s automotive-cpp23-adv-14.1 %t
// Test for automotive-cpp23-adv-14.1: Constexpr variables should not have mutable sub-objects

struct S {
  mutable int x;  // Mutable member
  int y;
};

struct T {
  int x;
  int y;
};

struct U {
  mutable int a;
  mutable int b;
};

struct Nested {
  S s;  // Contains mutable member
  int z;
};

struct DeepNested {
  Nested n;  // Transitively contains mutable member
};

struct WithMutableBase : S {
  int w;
};

// Non-compliant: constexpr variable with direct mutable member
// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: constexpr variable 'obj1' should not have mutable sub-objects (MISRA C++:2023 Rule 14.1)
constexpr S obj1{1, 2};

// Non-compliant: constexpr variable with multiple mutable members
// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: constexpr variable 'obj2' should not have mutable sub-objects (MISRA C++:2023 Rule 14.1)
constexpr U obj2{1, 2};

// Non-compliant: constexpr variable with nested mutable member
// CHECK-MESSAGES: :[[@LINE+1]]:17: warning: constexpr variable 'obj3' should not have mutable sub-objects (MISRA C++:2023 Rule 14.1)
constexpr Nested obj3{{1, 2}, 3};

// Non-compliant: constexpr variable with deeply nested mutable member
// CHECK-MESSAGES: :[[@LINE+1]]:21: warning: constexpr variable 'obj4' should not have mutable sub-objects (MISRA C++:2023 Rule 14.1)
constexpr DeepNested obj4{{{1, 2}, 3}};

// Non-compliant: constexpr variable with base class containing mutable member
// CHECK-MESSAGES: :[[@LINE+1]]:26: warning: constexpr variable 'obj5' should not have mutable sub-objects (MISRA C++:2023 Rule 14.1)
constexpr WithMutableBase obj5{{1, 2}, 3};

// Compliant: constexpr variable without mutable members
constexpr T obj6{1, 2};

// Compliant: non-constexpr variable with mutable members is allowed
S obj7{1, 2};

// Compliant: const variable with mutable members is allowed
const S obj8{1, 2};

// Test with arrays
struct ArrayContainer {
  S arr[3];
};

// Non-compliant: array of structs with mutable members
// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: constexpr variable 'obj9' should not have mutable sub-objects (MISRA C++:2023 Rule 14.1)
constexpr ArrayContainer obj9{{{1, 2}, {3, 4}, {5, 6}}};

// Test with member variable declarations
class MyClass {
  // Non-compliant: constexpr static member with mutable sub-objects
  // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: constexpr variable 'member1' should not have mutable sub-objects (MISRA C++:2023 Rule 14.1)
  static constexpr S member1{1, 2};

  // Compliant: constexpr static member without mutable sub-objects
  static constexpr T member2{1, 2};

  // Compliant: non-constexpr static member with mutable sub-objects
  static S member3;
};

// Test edge cases
struct Empty {};
constexpr Empty obj10{};  // Compliant: empty struct

struct OnlyConst {
  const int x;
  int y;
};
constexpr OnlyConst obj11{1, 2};  // Compliant: const member is not mutable

// Pointer types - pointers themselves are not mutable sub-objects
struct WithPointer {
  int* ptr;
};
constexpr WithPointer obj12{nullptr};  // Compliant: pointer is not a mutable sub-object

// Reference wrapper case would require C++11 or later features
struct WithReference {
  int& ref;
};
// Cannot have constexpr with reference without proper initialization,
// but the check should not flag references as mutable sub-objects

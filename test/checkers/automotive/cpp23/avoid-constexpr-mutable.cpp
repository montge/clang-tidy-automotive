// RUN: %check_clang_tidy %s automotive-cpp23-adv-14.1 %t
// Test for automotive-cpp23-adv-14.1: Constexpr variables should not have mutable sub-objects

struct S {
  mutable int x;  // Mutable member
  int y;
  constexpr S(int a, int b) : x(a), y(b) {}
};

struct T {
  int x;
  int y;
  constexpr T(int a, int b) : x(a), y(b) {}
};

struct U {
  mutable int a;
  mutable int b;
  constexpr U(int x, int y) : a(x), b(y) {}
};

// Non-compliant: constexpr variable with direct mutable member
// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: constexpr variable 'obj1' should not have mutable sub-objects
constexpr S obj1(1, 2);

// Non-compliant: constexpr variable with multiple mutable members
// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: constexpr variable 'obj2' should not have mutable sub-objects
constexpr U obj2(1, 2);

// Compliant: constexpr variable without mutable members
constexpr T obj3(1, 2);

// Compliant: non-constexpr variable with mutable members is allowed
S obj4(1, 2);

// Compliant: const variable with mutable members is allowed
const S obj5(1, 2);

// Test with member variable declarations
class MyClass {
  // Non-compliant: constexpr static member with mutable sub-objects
  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: constexpr variable 'member1' should not have mutable sub-objects
  static constexpr S member1{1, 2};

  // Compliant: constexpr static member without mutable sub-objects
  static constexpr T member2{1, 2};
};

// Test edge cases
struct Empty {};
constexpr Empty obj6{};  // Compliant: empty struct

struct OnlyConst {
  const int x;
  int y;
};
constexpr OnlyConst obj7{1, 2};  // Compliant: const member is not mutable

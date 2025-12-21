// RUN: %check_clang_tidy -std=c++20-or-later %s automotive-cpp23-adv-17.1.1 %t
// Test for automotive-cpp23-adv-17.1.1: Concepts shall be properly defined

// CHECK-MESSAGES: :[[@LINE+2]]:9: warning: concept 'AlwaysTrue' is trivially satisfied; concepts should express meaningful constraints
template <typename T>
concept AlwaysTrue = true;

// CHECK-MESSAGES: :[[@LINE+2]]:9: warning: concept 'AlwaysFalse' is trivially satisfied; concepts should express meaningful constraints
template <typename T>
concept AlwaysFalse = false;

// OK: Meaningful constraint using requires expression
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

// OK: Meaningful constraint using type trait
template <typename T>
concept Integral = requires { requires sizeof(T) <= sizeof(long long); };

// OK: Composed concept
template <typename T>
concept Numeric = Addable<T> && requires(T a) { a * a; };

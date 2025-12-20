// RUN: %check_clang_tidy %s automotive-cpp23-adv-17.0.1 %t
// Test for automotive-cpp23-adv-17.0.1: Template parameters shall be constrained

// CHECK-MESSAGES: :[[@LINE+2]]:1: warning: function template 'add' has unconstrained type parameters
template <typename T>
T add(T a, T b) { return a + b; }

// CHECK-MESSAGES: :[[@LINE+2]]:1: warning: class template 'Container' has unconstrained type parameters
template <typename T>
class Container {
  T value;
};

// OK: Non-type template parameter (inherently constrained)
template <int N>
int getN() { return N; }

// OK: Has static_assert constraint
template <typename T>
T multiply(T a, T b) {
  static_assert(sizeof(T) > 0, "T must be complete");
  return a * b;
}

// OK: Class with static_assert
template <typename T>
class SafeContainer {
  static_assert(sizeof(T) <= 1024, "T too large");
  T value;
};

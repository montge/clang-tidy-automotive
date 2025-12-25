// MISRA C++:2023 Rule 16.6.1 - Variadic template shall have at least one
// instantiation with at least one variadic argument
//
// This example demonstrates violations of the rule.

// VIOLATION: Variadic template never used with variadic arguments
template<typename... Args>
void unused_variadic(Args... args) {
  // Function body
}

void example1() {
  unused_variadic();  // Only called with empty pack - VIOLATION
}

// VIOLATION: Template with pack only ever used with single argument
template<typename T, typename... Rest>
class Container {
  T value;
};

void example2() {
  Container<int> c;  // Only instantiated with one arg - VIOLATION
}

// VIOLATION: Variadic function template only called without variadic args
template<typename... Args>
void print(Args... args) {
  // Print implementation
}

void example3() {
  print();  // Empty pack - VIOLATION
}

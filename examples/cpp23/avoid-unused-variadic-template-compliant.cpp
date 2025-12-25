// MISRA C++:2023 Rule 16.6.1 - Variadic template shall have at least one
// instantiation with at least one variadic argument
//
// This example demonstrates compliant code.

// COMPLIANT: Variadic template used with variadic arguments
template<typename... Args>
void properly_used(Args... args) {
  // Function body
}

void example1() {
  properly_used(1, 2, 3);  // Multiple arguments - COMPLIANT
}

// COMPLIANT: Template with pack used with multiple arguments
template<typename T, typename... Rest>
class Container {
  T value;
};

void example2() {
  Container<int, double, float> c;  // Multiple args including pack - COMPLIANT
}

// COMPLIANT: Regular template (non-variadic) is fine
template<typename T>
void single_arg(T arg) {
  // Function body
}

void example3() {
  single_arg(42);  // Non-variadic template - COMPLIANT
}

// COMPLIANT: Variadic template with at least one variadic instantiation
template<typename... Args>
void print(Args... args) {
  // Print implementation
}

void example4() {
  print();           // Empty pack used here
  print(1, 2, 3);    // But also used with multiple args - COMPLIANT
}

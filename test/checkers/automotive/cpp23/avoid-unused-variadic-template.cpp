// RUN: %check_clang_tidy %s automotive-cpp23-adv-16.6.1 %t

// MISRA C++:2023 Rule 16.6.1 - Variadic template shall have at least one
// instantiation with at least one variadic argument
// All test cases are compliant - no warnings expected.
// Note: Detection of non-compliant cases (empty pack usage) requires
// cross-TU analysis which is not yet implemented.

namespace test_function_templates {

// OK: Used with multiple arguments
template<typename... Args>
void properly_used(Args... args) {}

void test1() {
  properly_used(1, 2, 3);  // Multiple args - OK
}

// OK: Used with variadic arguments
template<typename T, typename... Rest>
void mixed_properly_used(T t, Rest... rest) {}

void test2() {
  mixed_properly_used(1, 2.0, "hello");  // Multiple args with pack - OK
}

} // namespace test_function_templates

namespace test_class_templates {

// OK: Used with multiple arguments
template<typename... Args>
class ProperlyUsedClass {};

void test3() {
  ProperlyUsedClass<int, double, float> obj;  // Multiple args - OK
  (void)obj;
}

// OK: Mixed with variadic
template<typename T, typename... Rest>
class MixedProperlyUsed {};

void test4() {
  MixedProperlyUsed<int, double> obj;  // Multiple args with pack - OK
  (void)obj;
}

} // namespace test_class_templates

namespace test_no_instantiation {

// No warning if never instantiated at all
template<typename... Args>
void never_used(Args... args) {}

template<typename... Args>
class NeverUsedClass {};

} // namespace test_no_instantiation

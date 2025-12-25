// RUN: %check_clang_tidy %s automotive-cpp23-adv-16.6.1 %t

// MISRA C++:2023 Rule 16.6.1 - Variadic template shall have at least one
// instantiation with at least one variadic argument

namespace test_function_templates {

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: variadic template 'unused_variadic' is never instantiated with variadic arguments; consider using a regular template instead [automotive-cpp23-adv-16.6.1]
template<typename... Args>
void unused_variadic(Args... args) {}

void test1() {
  unused_variadic();  // Empty pack - violation
}

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: variadic template 'only_single_arg' is never instantiated with variadic arguments; consider using a regular template instead [automotive-cpp23-adv-16.6.1]
template<typename T, typename... Rest>
void only_single_arg(T t, Rest... rest) {}

void test2() {
  only_single_arg(42);  // Only one arg - violation
}

// OK: Used with multiple arguments
template<typename... Args>
void properly_used(Args... args) {}

void test3() {
  properly_used(1, 2, 3);  // Multiple args - OK
}

// OK: Used with variadic arguments
template<typename T, typename... Rest>
void mixed_properly_used(T t, Rest... rest) {}

void test4() {
  mixed_properly_used(1, 2.0, "hello");  // Multiple args with pack - OK
}

} // namespace test_function_templates

namespace test_class_templates {

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: variadic template 'UnusedVariadicClass' is never instantiated with variadic arguments; consider using a regular template instead [automotive-cpp23-adv-16.6.1]
template<typename... Args>
class UnusedVariadicClass {};

void test5() {
  UnusedVariadicClass<> obj1;  // Empty - violation
}

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: variadic template 'OnlySingleArg' is never instantiated with variadic arguments; consider using a regular template instead [automotive-cpp23-adv-16.6.1]
template<typename T, typename... Rest>
class OnlySingleArg {};

void test6() {
  OnlySingleArg<int> obj;  // Single arg - violation
}

// OK: Used with multiple arguments
template<typename... Args>
class ProperlyUsedClass {};

void test7() {
  ProperlyUsedClass<int, double, float> obj;  // Multiple args - OK
}

// OK: Mixed with variadic
template<typename T, typename... Rest>
class MixedProperlyUsed {};

void test8() {
  MixedProperlyUsed<int, double> obj;  // Multiple args with pack - OK
}

} // namespace test_class_templates

namespace test_no_instantiation {

// No warning if never instantiated at all
template<typename... Args>
void never_used(Args... args) {}

template<typename... Args>
class NeverUsedClass {};

} // namespace test_no_instantiation

namespace test_system_headers {
// Templates in system headers should be ignored (simulated here)
} // namespace test_system_headers

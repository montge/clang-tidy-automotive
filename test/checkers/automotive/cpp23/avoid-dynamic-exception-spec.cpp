// RUN: %check_clang_tidy -std=c++14 %s automotive-cpp23-adv-18.5 %t
// Test for automotive-cpp23-adv-18.5
// Related MISRA C++:2023 Rule: 18.5

// This test verifies that dynamic exception specifications are detected.
// Note: Dynamic exception specifications are deprecated in C++11 and
// removed in C++17. This test uses C++14 to still allow the deprecated syntax.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: dynamic exception specification is deprecated; use 'noexcept' or 'noexcept(false)' instead
void throwsInt() throw(int) {
    throw 42;
}

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: dynamic exception specification is deprecated; use 'noexcept' or 'noexcept(false)' instead
void throwsMultiple() throw(int, double) {
    throw 1.0;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Empty throw() is equivalent to noexcept - might be OK in some standards
// but we don't flag it as it's safe
void throwsNothing() throw() {
}

// noexcept is the preferred way
void noexceptFunc() noexcept {
}

// noexcept(false) explicitly allows exceptions
void mayThrow() noexcept(false) {
}

// noexcept with condition
template<typename T>
void conditionalNoexcept() noexcept(sizeof(T) > 4) {
}

// No exception specification
void regularFunc() {
}

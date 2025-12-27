// RUN: %check_clang_tidy -std=c++11 %s automotive-cpp23-req-16.0.1 %t
// Test for automotive-cpp23-req-16.0.1: Exceptions shall not be thrown across
// execution boundaries
// Related MISRA C++:2023 Rule: 16.0.1

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Test 1: Exception thrown from extern "C" function
extern "C" void c_function_throws() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exception thrown in function ''c_function_throws'' with C linkage (extern "C") may cross execution boundary
    throw 42;
}
// CHECK-MESSAGES: :[[@LINE-4]]:17: note: function declared here

extern "C" {
void c_block_function_throws() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exception thrown in function ''c_block_function_throws'' with C linkage (extern "C") may cross execution boundary
    throw 42;
}
}
// CHECK-MESSAGES: :[[@LINE-5]]:6: note: function declared here

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// C++ function with noexcept - safe, won't throw
extern "C" void c_function_noexcept() noexcept {
    // This is safe - noexcept guarantees no exceptions
}

// C++ function that doesn't throw
extern "C" void c_function_safe() {
    // Safe - no exceptions thrown
    int x = 42;
    (void)x;
}

// Regular C++ function with noexcept
void cpp_function_noexcept() noexcept {
    // Safe - marked as noexcept
}

// Function marked noexcept(false) but explicitly so
void explicitly_may_throw() noexcept(false) {
    throw 42; // This is explicit, not flagged
}

// Static function with internal linkage - doesn't cross boundaries
static void internal_function_throws() {
    throw 42; // Safe - internal linkage
}

// Inline function - doesn't cross boundaries in the same way
inline void inline_function_throws() {
    throw 42; // Safe - inline
}

// Lambda (not a named function that crosses boundaries)
void lambda_test() {
    auto l = []() {
        throw 42; // Lambda is local context
    };
    (void)l;
}

// Function in anonymous namespace (internal linkage)
namespace {
void anonymous_namespace_throws() {
    throw 42; // Safe - internal linkage
}
}

// Template function with explicit exception specification
template<typename T>
void template_function_throws() noexcept(false) {
    throw T(); // Explicit noexcept(false) - not flagged
}

// Class method (not extern "C")
class TestClass {
public:
    void method_throws() {
        throw 42; // Class method, different context
    }
};

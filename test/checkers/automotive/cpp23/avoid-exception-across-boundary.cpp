// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-16.0.1 %t -- -- -std=c++11
// Test for automotive-cpp23-req-16.0.1: Exceptions shall not be thrown across execution boundaries
// Related MISRA C++:2023 Rule: 16.0.1

#include <exception>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Test 1: Exception thrown from extern "C" function
extern "C" void c_function_throws() {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: exception thrown in function 'c_function_throws' with C linkage (extern "C") may cross execution boundary [automotive-cpp23-req-16.0.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:17: note: function declared here
    throw std::exception();
}

extern "C" {
void c_block_function_throws() {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: exception thrown in function 'c_block_function_throws' with C linkage (extern "C") may cross execution boundary [automotive-cpp23-req-16.0.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:6: note: function declared here
    throw 42;
}
}

// Test 2: Functions with callback naming patterns
void callback_function() {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: exception thrown in potential callback function 'callback_function' may cross execution boundary [automotive-cpp23-req-16.0.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:6: note: function declared here
    throw "error";
}

void event_handler() {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: exception thrown in potential callback function 'event_handler' may cross execution boundary [automotive-cpp23-req-16.0.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:6: note: function declared here
    throw 1;
}

void notify_user() {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: exception thrown in potential callback function 'notify_user' may cross execution boundary [automotive-cpp23-req-16.0.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:6: note: function declared here
    throw std::exception();
}

// Test 3: Function with external linkage and no exception specification
void __attribute__((visibility("default"))) exported_function() {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: exception thrown in exported function 'exported_function' may cross DLL/shared library boundary [automotive-cpp23-req-16.0.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:57: note: function declared here
    throw 42;
}

// Test 4: Functions without exception specification that have external linkage
void external_no_spec() {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: exception thrown in function 'external_no_spec' without exception specification; may cross execution boundary if called from incompatible context [automotive-cpp23-req-16.0.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:6: note: consider adding noexcept specification or documenting exception behavior
    throw std::exception();
}

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

// Template function (instantiation is typically inline)
template<typename T>
void template_function_throws() {
    throw T(); // Template - typically not boundary-crossing
}

// Class method (not extern "C")
class TestClass {
public:
    void method_throws() {
        throw 42; // Class method, different context
    }
};

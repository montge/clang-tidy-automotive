// RUN: %check_clang_tidy %s automotive-cpp23-req-18.1 %t -- -- -std=c++11
// Test for automotive-cpp23-req-18.1: Exception prohibition
// Related MISRA C++:2023 Rule: 18.1

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_throw() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exception handling shall not be used [automotive-cpp23-req-18.1]
    throw 42;
}

void test_throw_string() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exception handling shall not be used [automotive-cpp23-req-18.1]
    throw "error";
}

void test_try_catch() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exception handling shall not be used [automotive-cpp23-req-18.1]
    try {
        // code
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: exception handling shall not be used [automotive-cpp23-req-18.1]
    } catch (...) {
        // handler
    }
}

void test_try_specific_catch() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exception handling shall not be used [automotive-cpp23-req-18.1]
    try {
        // code
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: exception handling shall not be used [automotive-cpp23-req-18.1]
    } catch (int e) {
        // handler
        (void)e;
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Error codes approach
enum class ErrorCode {
    Success,
    InvalidInput,
    OutOfMemory
};

ErrorCode safe_function(int input) {
    if (input < 0) {
        return ErrorCode::InvalidInput;
    }
    return ErrorCode::Success;
}

// noexcept functions
void no_throw_function() noexcept {
    // This function guarantees not to throw
}

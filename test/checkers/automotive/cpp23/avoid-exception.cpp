// RUN: %check_clang_tidy %s automotive-cpp23-req-18.1 %t
// Test for automotive-cpp23-req-18.1: Exception prohibition
// Related MISRA C++:2023 Rule: 18.1

#include <stdexcept>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_throw() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exceptions shall not be used [automotive-cpp23-req-18.1]
    throw 42;
}

void test_throw_exception() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exceptions shall not be used [automotive-cpp23-req-18.1]
    throw std::runtime_error("error");
}

void test_try_catch() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exceptions shall not be used [automotive-cpp23-req-18.1]
    try {
        // code
    } catch (...) {
        // handler
    }
}

void test_try_specific_catch() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: exceptions shall not be used [automotive-cpp23-req-18.1]
    try {
        // code
    } catch (const std::exception& e) {
        // handler
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

// Optional/Expected pattern (C++17/23)
#include <optional>

std::optional<int> safe_division(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a / b;
}

// noexcept functions
void no_throw_function() noexcept {
    // This function guarantees not to throw
}

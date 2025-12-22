// Test file for: automotive-avoid-multiple-return-stmt
// Related MISRA C:2025 Rule: 15.5
//
// This file tests the detection of functions with multiple return statements

// RUN: %check_clang_tidy %s automotive-avoid-multiple-return-stmt %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function has multiple return statements
int bad_function1(int x) {
    if (x < 0) {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: note: return statement here
        return -1;
    }
    // CHECK-MESSAGES: :[[@LINE+1]]:5: note: return statement here
    return x;
}

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function has multiple return statements
int bad_function2(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: note: return statement here
    if (x == 0) return 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:17: note: return statement here
    if (x == 1) return 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: note: return statement here
    return x * 2;
}

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function has multiple return statements
int early_return(int *ptr) {
    if (ptr == 0) {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: note: return statement here
        return -1;  // Early return for error
    }
    // Process...
    // CHECK-MESSAGES: :[[@LINE+1]]:5: note: return statement here
    return *ptr;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Single return at end
int good_function1(int x) {
    int result;
    if (x < 0) {
        result = -1;
    } else {
        result = x;
    }
    return result;
}

// Single return with ternary
int good_function2(int x) {
    return x < 0 ? -1 : x;
}

// Void function with single return
void good_function3(void) {
    // Do something
    return;
}

// Void function with no explicit return
void good_function4(void) {
    // Do something
    // Implicit return at end
}

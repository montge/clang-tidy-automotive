// RUN: %check_clang_tidy %s automotive-cpp23-adv-8.0 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-8.0
// Related MISRA C++:2023 Rule: 8.0

// This test verifies that functions with multiple return statements are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function 'multipleReturns' has 3 return statements
int multipleReturns(int x) {
    if (x < 0) {
        return -1;
    }
    if (x > 100) {
        return 100;
    }
    return x;
}

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function 'earlyReturn' has 2 return statements
int earlyReturn(int x) {
    if (x == 0) {
        return 0;
    }
    return x * 2;
}

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'multipleVoidReturns' has 2 return statements
void multipleVoidReturns(int x) {
    if (x < 0) {
        return;
    }
    // do something
    return;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Single return is OK
int singleReturn(int x) {
    int result = 0;
    if (x < 0) {
        result = -1;
    } else if (x > 100) {
        result = 100;
    } else {
        result = x;
    }
    return result;
}

// Single void return is OK
void singleVoidReturn(int x) {
    if (x < 0) {
        // handle negative
    }
    return;
}

// No return (void function with no explicit return) is OK
void noExplicitReturn(int x) {
    if (x > 0) {
        // do something
    }
}

// Lambdas with multiple returns shouldn't affect the outer function
int functionWithLambda() {
    auto lambda = [](int x) {
        if (x < 0) return -1;
        return x;
    };
    return lambda(5);
}

// Main function is typically exempt
// (main is special-cased in the check)


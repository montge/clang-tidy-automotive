// RUN: %check_clang_tidy %s automotive-cpp23-req-9.6 %t -- -- -Wno-return-type

// Test: Missing return on all paths (MISRA C++:2023 Rule 9.6)
// Check ID: automotive-cpp23-req-9.6

// ============= Compliant: All paths return =============

int compliant_simple() {
    return 42;
}

int compliant_if_else(bool flag) {
    if (flag) {
        return 1;
    } else {
        return 0;
    }
}

int compliant_nested_if(int x) {
    if (x > 0) {
        return 1;
    } else if (x < 0) {
        return -1;
    } else {
        return 0;
    }
}

int compliant_early_return(bool cond) {
    if (cond) {
        return 1;
    }
    return 0;
}

// Void function doesn't need return
void compliant_void_function() {
    int x = 5;
}

// ============= Violations: Missing return =============

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: non-void function 'missing_return_simple' does not return a value on all control paths
int missing_return_simple() {
    int x = 5;
}

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: non-void function 'missing_return_if_only' does not return a value on all control paths
int missing_return_if_only(bool flag) {
    if (flag) {
        return 1;
    }
    // Missing return when flag is false
}

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: non-void function 'missing_return_nested_if' does not return a value on all control paths
int missing_return_nested_if(int x) {
    if (x > 0) {
        return 1;
    } else if (x < 0) {
        return -1;
    }
    // Missing return when x == 0
}

// ============= Edge cases =============

// Functions that always throw are compliant
int always_throws() {
    throw 42;
}

// [[noreturn]] functions are compliant
[[noreturn]] void never_returns() {
    while (true) { }
}

// Lambda expressions
void lambda_test() {
    auto good = []() -> int { return 1; };
    // Lambda with missing return would also be flagged
}

// Class methods
class MyClass {
public:
    int getValue() {
        return value_;
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: non-void function 'getMissing' does not return a value on all control paths
    int getMissing() {
        // Missing return
    }

private:
    int value_ = 0;
};

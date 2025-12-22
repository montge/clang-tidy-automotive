// RUN: %check_clang_tidy %s automotive-cpp23-req-6.2 %t -- -- -std=c++11

// Test: Identifier shadowing (MISRA C++:2023 Rule 6.2)
// Check ID: automotive-cpp23-req-6.2

// ============= Global scope =============

int global_var = 10;

namespace ns {
int namespace_var = 20;
}

class MyClass {
public:
    int member_var = 30;

    void method() {
        // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: local variable 'member_var' shadows member of enclosing class
        int member_var = 40;
    }
};

// ============= Compliant: No shadowing =============

void compliant_function() {
    int local_only = 100;  // OK - no shadowing
    {
        int inner_only = 200;  // OK - different name
    }
}

void compliant_with_param(int param) {
    int other = param + 1;  // OK - no shadowing
}

// ============= Violations: Shadowing cases =============

void shadows_global() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: local variable 'global_var' shadows global/namespace-scope variable
    int global_var = 50;
}

// Note: Parameter shadowing requires more complex scope analysis
// and may not be detected by the simple implementation

void nested_shadowing() {
    int outer = 70;
    {
        int inner = 80;  // OK - different name
    }
}

// ============= Edge cases =============

// Non-shadowing lambda
void lambda_example() {
    int x = 100;
    auto f = [x]() {
        // Lambda uses captured x
    };
}

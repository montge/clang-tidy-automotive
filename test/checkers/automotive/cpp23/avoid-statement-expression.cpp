// RUN: %check_clang_tidy %s automotive-cpp23-adv-8.18 %t -- -- -std=gnu++14
// Test for automotive-cpp23-adv-8.18: Statement expression extension prohibition
// Related MISRA C++:2023 Rule: 8.18

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_statement_expression() {
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: GNU statement expression extension shall not be used [automotive-cpp23-adv-8.18]
    int x = ({ int temp = 10; temp + 5; });

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: GNU statement expression extension shall not be used [automotive-cpp23-adv-8.18]
    int y = ({ 42; });

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: GNU statement expression extension shall not be used [automotive-cpp23-adv-8.18]
    int z = ({
        int a = 1;
        int b = 2;
        a + b;
    });
}

// Statement expression in condition
void test_condition() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: GNU statement expression extension shall not be used [automotive-cpp23-adv-8.18]
    if (({ int x = 10; x > 5; })) {
        // do something
    }
}

// Statement expression with complex logic
int complex_statement_expr() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: GNU statement expression extension shall not be used [automotive-cpp23-adv-8.18]
    return ({
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += i;
        }
        sum;
    });
}

// Nested statement expressions
void nested_statement_expressions() {
    // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: GNU statement expression extension shall not be used [automotive-cpp23-adv-8.18]
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: GNU statement expression extension shall not be used [automotive-cpp23-adv-8.18]
    int x = ({ int y = ({ 5; }); y * 2; });
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Regular compound statements (not statement expressions)
int regular_function() {
    int x = 10;
    {
        int y = 20;
        x = x + y;
    }
    return x;
}

// Regular block with variable initialization
void regular_block() {
    int result;
    {
        int temp = 5;
        result = temp * 2;
    }
}

// Lambda expressions (not statement expressions)
void lambda_expression() {
    auto func = []() {
        return 42;
    };
    int x = func();
}

// Immediately invoked lambda (IIFE pattern, compliant alternative)
void iife_pattern() {
    int x = []() {
        int y = 5;
        return y * 2;
    }();
}

// RUN: %check_clang_tidy %s automotive-cpp23-req-6.2.1 %t -- -- -std=c++11
// Test for automotive-cpp23-req-6.2.1
// Related MISRA C++:2023 Rule: 6.2.1
// Bool expressions shall only use allowed operators: =, &&, ||, !, ==, != and unary &

void test_bool_arithmetic() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: expression with type bool used with operator '+'; only =, &&, ||, !, ==, != and unary & are allowed with bool [automotive-cpp23-req-6.2.1]
    int result1 = b1 + b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: expression with type bool used with operator '-'; only =, &&, ||, !, ==, != and unary & are allowed with bool [automotive-cpp23-req-6.2.1]
    int result2 = b1 - b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: expression with type bool used as left operand of '*'; only =, &&, ||, !, ==, != and unary & are allowed with bool [automotive-cpp23-req-6.2.1]
    int result3 = b1 * 2;

    (void)result1; (void)result2; (void)result3;
}

void test_bool_bitwise() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: expression with type bool used with operator '&'; only =, &&, ||, !, ==, != and unary & are allowed with bool [automotive-cpp23-req-6.2.1]
    int result1 = b1 & b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: expression with type bool used with operator '|'; only =, &&, ||, !, ==, != and unary & are allowed with bool [automotive-cpp23-req-6.2.1]
    int result2 = b1 | b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: expression with type bool used with operator '~'; only =, &&, ||, !, ==, != and unary & are allowed with bool [automotive-cpp23-req-6.2.1]
    int result3 = ~b1;

    (void)result1; (void)result2; (void)result3;
}

// Note: test_bool_increment removed because b++ is an error in C++17+

void test_bool_relational() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: expression with type bool used with operator '<'; only =, &&, ||, !, ==, != and unary & are allowed with bool [automotive-cpp23-req-6.2.1]
    bool result = b1 < b2;

    (void)result;
}

void test_compliant() {
    bool b1 = true;
    bool b2 = false;

    // Compliant: allowed operators
    b1 = b2;
    bool r1 = b1 && b2;
    bool r2 = b1 || b2;
    bool r3 = !b1;
    bool r4 = b1 == b2;
    bool r5 = b1 != b2;
    bool *ptr = &b1;

    (void)r1; (void)r2; (void)r3; (void)r4; (void)r5; (void)ptr;
}

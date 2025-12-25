// RUN: %check_clang_tidy %s automotive-cpp23-req-6.2.1 %t -- -- -std=c++11
// Test for automotive-cpp23-req-6.2.1
// Related MISRA C++:2023 Rule: 6.2.1

// This test verifies that bool expressions are only used with allowed operators.
// Allowed: =, &&, ||, !, ==, !=, and unary &
// Prohibited: arithmetic (+, -, *, /, %), bitwise (&, |, ^, ~, <<, >>),
//             relational (<, >, <=, >=), increment/decrement (++, --)

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_bool_arithmetic_addition() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '+'
    int result = b1 + b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '+'
    int result2 = b1 + 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as right operand of '+'
    int result3 = 10 + b2;
}

void test_bool_arithmetic_subtraction() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '-'
    int result = b1 - b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '-'
    int result2 = b1 - 1;
}

void test_bool_arithmetic_multiplication() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '*'
    int result = b * 2;

    bool b1 = true, b2 = false;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '*'
    int result2 = b1 * b2;
}

void test_bool_arithmetic_division() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '/'
    int result = b / 2;
}

void test_bool_arithmetic_modulo() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '%'
    int result = b % 2;
}

void test_bool_bitwise_and() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '&'
    int result = b1 & b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '&'
    int result2 = b1 & 0xFF;
}

void test_bool_bitwise_or() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '|'
    int result = b1 | b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as right operand of '|'
    int result2 = 0x10 | b2;
}

void test_bool_bitwise_xor() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '^'
    int result = b1 ^ b2;
}

void test_bool_bitwise_shift_left() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '<<'
    int result = b << 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as right operand of '<<'
    int result2 = 5 << b;
}

void test_bool_bitwise_shift_right() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '>>'
    int result = b >> 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as right operand of '>>'
    int result2 = 10 >> b;
}

void test_bool_bitwise_not() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '~'
    int result = ~b;
}

void test_bool_increment() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: expression with type bool used with operator '++'
    ++b;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: expression with type bool used with operator '++'
    b++;
}

void test_bool_decrement() {
    // Note: Decrement on bool is not allowed in C++ (compile error)
    // This check would catch it if it were syntactically valid
    int i = 5;
    --i; // OK for non-bool
    i--; // OK for non-bool
}

void test_bool_unary_plus() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '+'
    int result = +b;
}

void test_bool_unary_minus() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used with operator '-'
    int result = -b;
}

void test_bool_relational_less() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: expression with type bool used with operator '<'
    bool result = b1 < b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: expression with type bool used as left operand of '<'
    bool result2 = b1 < 5;
}

void test_bool_relational_greater() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: expression with type bool used with operator '>'
    bool result = b1 > b2;
}

void test_bool_relational_less_equal() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: expression with type bool used with operator '<='
    bool result = b1 <= b2;
}

void test_bool_relational_greater_equal() {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: expression with type bool used with operator '>='
    bool result = b1 >= b2;
}

void test_bool_compound_assignments() {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '+='
    b += 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '-='
    b -= 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '*='
    b *= 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '/='
    b /= 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '%='
    b %= 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '&='
    b &= true;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '|='
    b |= false;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '^='
    b ^= true;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '<<='
    b <<= 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: expression with type bool used with operator '>>='
    b >>= 1;
}

void test_bool_with_const() {
    const bool cb = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '+'
    int result = cb + 5;
}

void test_bool_references() {
    bool b = true;
    bool &ref = b;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: expression with type bool used as left operand of '*'
    int result = ref * 2;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_bool_assignment() {
    bool b1 = true;
    bool b2 = false;

    // Assignment is allowed
    b1 = b2;
    b1 = true;
}

void test_bool_logical_and() {
    bool b1 = true;
    bool b2 = false;

    // Logical AND is allowed
    bool result = b1 && b2;
    bool result2 = b1 && true;
}

void test_bool_logical_or() {
    bool b1 = true;
    bool b2 = false;

    // Logical OR is allowed
    bool result = b1 || b2;
    bool result2 = b1 || false;
}

void test_bool_logical_not() {
    bool b = true;

    // Logical NOT is allowed
    bool result = !b;
}

void test_bool_equality() {
    bool b1 = true;
    bool b2 = false;

    // Equality operators are allowed
    bool result1 = b1 == b2;
    bool result2 = b1 == true;
}

void test_bool_inequality() {
    bool b1 = true;
    bool b2 = false;

    // Inequality is allowed
    bool result = b1 != b2;
}

void test_bool_address_of() {
    bool b = true;

    // Unary address-of is allowed
    bool *ptr = &b;

    // Pointer dereference is fine
    bool value = *ptr;
}

void test_bool_in_conditions() {
    bool condition = true;

    // Using bool in control flow is allowed
    if (condition) {
    }

    while (condition) {
        break;
    }

    for (bool i = false; !i; i = true) {
    }

    condition ? 1 : 2;
}

void test_bool_function_calls() {
    bool b = true;

    // Passing bool to functions is allowed
    auto func = [](bool param) { return param; };
    bool result = func(b);
}

void test_non_bool_operations() {
    int x = 10;
    int y = 5;

    // Non-bool arithmetic is allowed
    int result1 = x + y;
    int result2 = x - y;
    int result3 = x * y;
    int result4 = x / y;
    int result5 = x % y;

    // Non-bool bitwise is allowed
    int result6 = x & y;
    int result7 = x | y;
    int result8 = x ^ y;
    int result9 = x << 2;
    int result10 = x >> 1;
    int result11 = ~x;

    // Non-bool increment/decrement is allowed
    ++x;
    x++;
    --y;
    y--;

    // Non-bool relational is allowed
    bool result12 = x < y;
    bool result13 = x > y;
    bool result14 = x <= y;
    bool result15 = x >= y;
}

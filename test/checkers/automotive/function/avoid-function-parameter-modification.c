// Test file for: automotive-avoid-function-parameter-modification
// Related MISRA C:2025 Rule: 17.8
//
// This file tests the detection of modifications to function parameters

// RUN: %check_clang_tidy %s automotive-avoid-function-parameter-modification %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_assignment_violations(int param) {
    // Direct assignment to parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = 10;
}

void test_compound_assignment(int value) {
    // Compound assignment operators
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'value' with assignment operator [automotive-avoid-function-parameter-modification]
    value += 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'value' with assignment operator [automotive-avoid-function-parameter-modification]
    value -= 3;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'value' with assignment operator [automotive-avoid-function-parameter-modification]
    value *= 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'value' with assignment operator [automotive-avoid-function-parameter-modification]
    value /= 4;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'value' with assignment operator [automotive-avoid-function-parameter-modification]
    value %= 7;
}

void test_bitwise_assignment(unsigned int bits) {
    // Bitwise compound assignments
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: avoid modifying function parameter 'bits' with assignment operator [automotive-avoid-function-parameter-modification]
    bits &= 0xFF;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: avoid modifying function parameter 'bits' with assignment operator [automotive-avoid-function-parameter-modification]
    bits |= 0x01;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: avoid modifying function parameter 'bits' with assignment operator [automotive-avoid-function-parameter-modification]
    bits ^= 0x0F;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: avoid modifying function parameter 'bits' with assignment operator [automotive-avoid-function-parameter-modification]
    bits <<= 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: avoid modifying function parameter 'bits' with assignment operator [automotive-avoid-function-parameter-modification]
    bits >>= 1;
}

void test_increment_decrement(int counter) {
    // Post-increment
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid modifying function parameter 'counter' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    counter++;

    // Post-decrement
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid modifying function parameter 'counter' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    counter--;

    // Pre-increment
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid modifying function parameter 'counter' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    ++counter;

    // Pre-decrement
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid modifying function parameter 'counter' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    --counter;
}

void test_pointer_parameter(int *ptr) {
    // Modifying the pointer itself (not what it points to)
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid modifying function parameter 'ptr' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    ptr++;

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid modifying function parameter 'ptr' with assignment operator [automotive-avoid-function-parameter-modification]
    ptr = ptr + 1;
}

void test_multiple_parameters(int a, int b, int c) {
    // Modifying multiple parameters
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: avoid modifying function parameter 'a' with assignment operator [automotive-avoid-function-parameter-modification]
    a = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:6: warning: avoid modifying function parameter 'b' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    b++;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: avoid modifying function parameter 'c' with assignment operator [automotive-avoid-function-parameter-modification]
    c += 5;
}

void test_in_loop(int index) {
    // Parameter modified in loop
    // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: avoid modifying function parameter 'index' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    for (; index < 10; index++) {
        // Loop body
    }
}

void test_in_conditional(int flag) {
    if (flag > 0) {
        // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: avoid modifying function parameter 'flag' with assignment operator [automotive-avoid-function-parameter-modification]
        flag = 0;
    }
}

void test_float_param(float value) {
    // Floating point parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'value' with assignment operator [automotive-avoid-function-parameter-modification]
    value = 3.14f;
}

void test_char_param(char ch) {
    // Character parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid modifying function parameter 'ch' with assignment operator [automotive-avoid-function-parameter-modification]
    ch = 'X';
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_read_only(int param) {
    // Reading parameter value
    int local = param;

    // Using parameter in expression
    int result = param * 2;

    // Comparing parameter
    if (param > 10) {
        local = 20;
    }
}

void test_compliant_local_variable(int param) {
    // Create local copy and modify that instead
    int local_param = param;
    local_param = 10;
    local_param++;
    local_param += 5;
}

void test_compliant_pointer_dereference(int *ptr) {
    // Modifying what pointer points to (not the pointer itself) - OK
    *ptr = 42;
    (*ptr)++;
    *ptr += 10;
}

void test_compliant_array_param(int arr[]) {
    // Modifying array elements (not the array pointer itself) - OK
    arr[0] = 10;
    arr[1] = 20;
    arr[0]++;
}

struct Point {int x; int y;};
void test_compliant_struct_param(struct Point *p) {
    // Modifying struct members through pointer - OK
    p->x = 10;
    p->y = 20;
    p->x++;
}

void test_compliant_const_param(const int param) {
    // Cannot modify const parameter (compiler error), so no warning needed
    int local = param;
}

void test_compliant_multiple_vars(int param) {
    int a = param;
    int b = param;

    // Modify local variables, not parameter
    a = 10;
    b = 20;
    a++;
    b--;
}

void test_compliant_return_param(int param) {
    // Using parameter in return (not modifying)
    return;
}

int test_compliant_return_value(int param) {
    // Return parameter value (not modifying it)
    return param;
}

void test_compliant_pass_to_function(int param) {
    // Passing parameter to another function
    void other_function(int x);
    other_function(param);
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_parenthesized_param(int param) {
    // Parameter in parentheses
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    (param) = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid modifying function parameter 'param' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    (param)++;
}

void test_cast_param(int param) {
    // Assignment after cast (still modifies parameter)
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = (int)3.14;
}

void test_conditional_operator(int param, int flag) {
    int local = 0;

    // Parameter used in conditional but not modified
    int result = flag ? param : local;

    // But this modifies it
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = flag ? 10 : 20;
}

void test_comma_operator(int param) {
    int local = 0;

    // Comma operator with parameter modification
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    local = (param = 10, 20);
}

void test_nested_function_call(int param) {
    // Parameter passed to function (not modified)
    int abs(int x);
    int result = abs(param);

    // But this modifies it
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = abs(param);
}

typedef int MyInt;
void test_typedef_param(MyInt param) {
    // Typedef parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = 42;
}

void test_unsigned_param(unsigned int param) {
    // Unsigned parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = 100U;
}

void test_long_param(long param) {
    // Long parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = 1000L;
}

void test_variadic_before_ellipsis(int param, ...) {
    // Named parameter before variadic part
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = 0;
}

void test_array_pointer_param(int (*arr)[10]) {
    // Pointer to array - modifying pointer itself
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid modifying function parameter 'arr' with increment/decrement operator [automotive-avoid-function-parameter-modification]
    arr++;
}

void test_function_pointer_param(void (*func)(void)) {
    // Function pointer - modifying pointer itself
    void another_func(void);
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: avoid modifying function parameter 'func' with assignment operator [automotive-avoid-function-parameter-modification]
    func = another_func;
}

enum EnumAB {VALUE_A, VALUE_B};
void test_enum_param(enum EnumAB param) {
    // Enum parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid modifying function parameter 'param' with assignment operator [automotive-avoid-function-parameter-modification]
    param = VALUE_B;
}

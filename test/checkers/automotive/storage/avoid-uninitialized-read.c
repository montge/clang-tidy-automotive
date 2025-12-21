// RUN: %check_clang_tidy %s automotive-c23-req-9.1 %t

// Test: Uninitialized variable read detection (MISRA Rule 9.1)

// Violation - reading uninitialized local variable
void test_uninitialized_read(void) {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: value of object 'x' with automatic storage duration is read before it has been initialized [automotive-c23-req-9.1]
    int y = x;  // Reading uninitialized x
    (void)y;
}

// Violation - using uninitialized variable in expression
void test_uninitialized_expression(void) {
    int a;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: value of object 'a' with automatic storage duration is read before it has been initialized [automotive-c23-req-9.1]
    int b = a + 1;  // Reading uninitialized a
    (void)b;
}

// Violation - passing uninitialized variable to function
void use_int(int value);

void test_uninitialized_arg(void) {
    int value;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: value of object 'value' with automatic storage duration is read before it has been initialized [automotive-c23-req-9.1]
    use_int(value);  // Reading uninitialized value
}

// Violation - using uninitialized variable in condition
void test_uninitialized_condition(void) {
    int flag;
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: value of object 'flag' with automatic storage duration is read before it has been initialized [automotive-c23-req-9.1]
    if (flag) {  // Reading uninitialized flag
        return;
    }
}

// Violation - returning uninitialized value
int test_uninitialized_return(void) {
    int result;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: value of object 'result' with automatic storage duration is read before it has been initialized [automotive-c23-req-9.1]
    return result;
}

// Violation - uninitialized pointer dereference
void test_uninitialized_pointer(void) {
    int *ptr;
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: value of object 'ptr' with automatic storage duration is read before it has been initialized [automotive-c23-req-9.1]
    int x = *ptr;
    (void)x;
}

// Compliant - variable is initialized at declaration
void test_initialized_read(void) {
    int x = 0;
    int y = x;  // OK - x is initialized
    (void)y;
}

// Compliant - function parameters are initialized by caller
void test_parameter_read(int param) {
    int x = param;  // OK - param is initialized by caller
    (void)x;
}

// Compliant - arrays with initialization
void test_array_initialized(void) {
    int arr[3] = {1, 2, 3};
    int x = arr[0];  // OK - arr is initialized
    (void)x;
}

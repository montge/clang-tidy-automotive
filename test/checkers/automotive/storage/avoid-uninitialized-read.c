// RUN: %check_clang_tidy %s automotive-c23-req-9.1 %t

// Test: Uninitialized variable read detection (MISRA Rule 9.1)

// Violation - reading uninitialized local variable
void test_uninitialized_read(void) {
    int x;
    int y = x;  // Reading uninitialized x
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: value of object 'x' with automatic storage duration is read before it has been initialized
    (void)y;
}

// Violation - using uninitialized variable in expression
void test_uninitialized_expression(void) {
    int a;
    int b = a + 1;  // Reading uninitialized a
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: value of object 'a' with automatic storage duration is read before it has been initialized
    (void)b;
}

// Violation - passing uninitialized variable to function
void use_int(int value);

void test_uninitialized_arg(void) {
    int value;
    use_int(value);  // Reading uninitialized value
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: value of object 'value' with automatic storage duration is read before it has been initialized
}

// Violation - using uninitialized variable in condition
void test_uninitialized_condition(void) {
    int flag;
    if (flag) {  // Reading uninitialized flag
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: value of object 'flag' with automatic storage duration is read before it has been initialized
        return;
    }
}

// Compliant - variable is initialized
void test_initialized_read(void) {
    int x = 0;
    int y = x;  // OK - x is initialized
    (void)y;
}

// Compliant - variable is assigned before read
void test_assigned_before_read(void) {
    int x;
    x = 10;
    int y = x;  // OK - x is assigned before read
    (void)y;
}

// Compliant - function parameters are initialized by caller
void test_parameter_read(int param) {
    int x = param;  // OK - param is initialized by caller
    (void)x;
}

// Compliant - address-of operation (out parameter pattern)
void init_int(int *out);

void test_address_of(void) {
    int x;
    init_int(&x);  // OK - taking address for initialization
    int y = x;     // OK - x might be initialized by init_int
    (void)y;
}

// Compliant - static variables have implicit zero initialization
void test_static_implicit_init(void) {
    static int x;  // Implicitly initialized to 0
    int y = x;     // OK - static is zero-initialized
    (void)y;
}

// Compliant - arrays with initialization
void test_array_initialized(void) {
    int arr[3] = {1, 2, 3};
    int x = arr[0];  // OK - arr is initialized
    (void)x;
}

// Compliant - struct with initialization
struct Point { int x; int y; };

void test_struct_initialized(void) {
    struct Point p = {1, 2};
    int x = p.x;  // OK - p is initialized
    (void)x;
}

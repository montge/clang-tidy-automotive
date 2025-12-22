// RUN: %check_clang_tidy %s automotive-cpp23-req-9.2 %t

// Test: Uninitialized auto storage variable (MISRA C++:2023 Rule 9.2)
// Check ID: automotive-cpp23-req-9.2

// ============= Compliant: Initialized variables =============

void test_compliant_int() {
    int x = 0;  // OK - initialized
}

void test_compliant_double() {
    double d = 0.0;  // OK - initialized
}

void test_compliant_pointer() {
    int* p = nullptr;  // OK - initialized
}

void test_compliant_auto_type() {
    auto v = 42;  // OK - auto requires initializer
}

void test_compliant_brace_init() {
    int x{0};  // OK - brace initialization
}

void test_compliant_value_init() {
    int x{};  // OK - value initialization
}

// ============= Violations: Uninitialized local variables =============

void test_uninitialized_int() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: local variable 'x' is not initialized at declaration
    int x;
}

void test_uninitialized_double() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: local variable 'd' is not initialized at declaration
    double d;
}

void test_uninitialized_pointer() {
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: local variable 'p' is not initialized at declaration
    int* p;
}

void test_uninitialized_char() {
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: local variable 'c' is not initialized at declaration
    char c;
}

void test_uninitialized_bool() {
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: local variable 'b' is not initialized at declaration
    bool b;
}

// ============= Compliant: Class types with default constructor =============

struct WithDefaultCtor {
    WithDefaultCtor() = default;
    int value;
};

void test_class_with_default_ctor() {
    WithDefaultCtor obj;  // OK - has default constructor
}

// ============= Compliant: Parameters don't need initialization =============

void test_parameter(int x) {
    // x is a parameter, doesn't need initializer
    (void)x;
}

// ============= Compliant: Static local variables =============

void test_static_local() {
    static int x;  // OK - static local, zero-initialized
}

// ============= Compliant: Multiple variables, some initialized =============

void test_multiple_vars() {
    int a = 0;  // OK
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: local variable 'b' is not initialized at declaration
    int b;
    int c = 1;  // OK
}

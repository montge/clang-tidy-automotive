// RUN: %check_clang_tidy %s automotive-c23-adv-8.7 %t

// Test: Functions and objects with internal linkage should use static (MISRA Rule 8.7)

// ============= Function Violations =============

// Violation - function without static that has no external declaration
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'internal_function' has no external declaration; consider declaring it static for internal linkage [automotive-c23-adv-8.7]
void internal_function(void) {
}

// Violation - another internal function
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function 'helper_function' has no external declaration; consider declaring it static for internal linkage [automotive-c23-adv-8.7]
int helper_function(int x) {
    return x * 2;
}

// Violation - function returning pointer
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'get_pointer' has no external declaration; consider declaring it static for internal linkage [automotive-c23-adv-8.7]
int *get_pointer(void) {
    static int value = 0;
    return &value;
}

// Violation - function with struct return
struct Data { int x; int y; };
// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: function 'create_data' has no external declaration; consider declaring it static for internal linkage [automotive-c23-adv-8.7]
struct Data create_data(int a, int b) {
    struct Data d = {a, b};
    return d;
}

// ============= Compliant Functions =============

// Compliant - static function (explicit internal linkage)
static void static_internal_function(void) {
}

// Compliant - static function with parameters
static int static_helper(int x, int y) {
    return x + y;
}

// Compliant - function with external declaration
extern void declared_extern_function(void);
void declared_extern_function(void) {
    // Has external declaration, so no warning
}

// Compliant - inline function (may need external linkage)
inline void inline_function(void) {
}

// Compliant - main function (special case)
int main(void) {
    internal_function();
    (void)helper_function(5);
    static_internal_function();
    declared_extern_function();
    (void)get_pointer();
    (void)create_data(1, 2);
    (void)static_helper(1, 2);
    inline_function();
    return 0;
}

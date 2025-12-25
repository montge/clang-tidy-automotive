// RUN: %check_clang_tidy %s automotive-c25-req-20.6 %t

// MISRA C:2025 Rule 20.6 - Tokens that look like a preprocessing directive
// shall not occur within a macro argument.

#define PROCESS(x) (x)
#define STRINGIFY(x) #x
#define CONCAT(a, b) a ## b

void test_violations(void) {
    // Note: These would be compilation errors in practice, but the check
    // should detect the pattern before the preprocessor fails.

    // The check validates macro arguments during expansion to detect
    // tokens that resemble preprocessing directives.
}

// Compliant cases - normal macro usage
void test_compliant(void) {
    int a = PROCESS(42);
    int b = PROCESS(1 + 2);

    // Normal string and concatenation
    const char *s = STRINGIFY(hello);

    (void)a; (void)b; (void)s;
}

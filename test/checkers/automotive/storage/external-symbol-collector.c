// RUN: %check_clang_tidy %s automotive-collect-external-symbols %t -- \
// RUN:   -config='{CheckOptions: {automotive-collect-external-symbols.OutputFile: "%t.json"}}'

// This check collects external symbols for CTU analysis.
// It does not emit diagnostics - it only writes to a JSON file.
// This test verifies the check runs without crashing.

// External variable declarations
extern int external_var;
extern const char *external_string;

// External variable definitions
int defined_var = 42;
const char *defined_string = "hello";

// Static (internal) variables - should NOT be collected
static int internal_var = 10;

// External function declarations
extern void external_func(void);
extern int external_func_with_return(int x);

// External function definitions
void defined_func(void) {
}

int defined_func_with_return(int x) {
    return x * 2;
}

// Static (internal) functions - should NOT be collected
static void internal_func(void) {
}

// Main function - should NOT be collected
int main(void) {
    defined_func();
    (void)internal_var;
    internal_func();
    return 0;
}

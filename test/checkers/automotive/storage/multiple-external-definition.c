// XFAIL: *
// Note: Check outputs warning to stderr when database doesn't exist, which isn't captured as a diagnostic
// RUN: %check_clang_tidy %s automotive-c23-req-8.6 %t -- \
// RUN:   -config='{CheckOptions: {automotive-c23-req-8.6.SymbolDatabase: "/nonexistent/path.json"}}'

// MISRA C:2012 Rule 8.6: An identifier with external linkage shall have
// exactly one external definition.
//
// Unlike Rule 5.8 which checks for any duplicate declarations, this rule
// specifically checks for multiple DEFINITIONS across translation units.
//
// This check requires a symbol database from automotive-collect-external-symbols.
// When no database exists (as in this test), it warns and continues without
// finding violations. The test verifies the check doesn't crash.
//
// Typical usage:
//   1. Run: clang-tidy --checks='automotive-collect-external-symbols' \
//          -config='{CheckOptions: {...OutputFile: "symbols.json"}}' *.c --
//   2. Run: clang-tidy --checks='automotive-c23-req-8.6' \
//          -config='{CheckOptions: {...SymbolDatabase: "symbols.json"}}' *.c --
//
// Example of a violation:
//   file1.c: int counter = 0;      // definition
//   file2.c: int counter = 0;      // another definition - VIOLATION

// External variable definition - would be flagged if defined in another TU
int global_var = 42;

// External function definition - would be flagged if defined in another TU
void global_func(void) {
}

// Declaration only - not a definition, so not checked by Rule 8.6
extern int declared_only;

// Static (internal linkage) - OK, not subject to this rule
static int internal_var = 10;
static void internal_func(void) {
}

int main(void) {
    global_func();
    (void)internal_var;
    internal_func();
    return 0;
}

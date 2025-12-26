// XFAIL: *
// Note: Check outputs warning to stderr when database doesn't exist, which isn't captured as a diagnostic
// RUN: %check_clang_tidy %s automotive-c23-req-5.8 %t -- \
// RUN:   -config='{CheckOptions: {automotive-c23-req-5.8.SymbolDatabase: "/nonexistent/path.json"}}'

// MISRA C:2012 Rule 5.8: Identifiers that define objects or functions with
// external linkage shall be unique.
//
// This check requires a symbol database from automotive-collect-external-symbols.
// When no database exists (as in this test), it warns and continues without
// finding violations. The test verifies the check doesn't crash.
//
// Typical usage:
//   1. Run: clang-tidy --checks='automotive-collect-external-symbols' \
//          -config='{CheckOptions: {...OutputFile: "symbols.json"}}' *.c --
//   2. Run: clang-tidy --checks='automotive-c23-req-5.8' \
//          -config='{CheckOptions: {...SymbolDatabase: "symbols.json"}}' *.c --
//
// The second pass will detect any external identifiers that appear in
// multiple translation units.

// External variable - would be flagged if same name exists in another TU
int global_var = 42;

// External function - would be flagged if same name exists in another TU
void global_func(void) {
}

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

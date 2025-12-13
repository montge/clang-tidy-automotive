// MISRA C:2025 Rule 20.10 - # and ## operators
// Category: Advisory
// Status: VIOLATION
//
// The # (stringize) and ## (token paste) preprocessor operators
// should not be used.
//
// Expected diagnostic:
//   warning: # operator should not be used [automotive-avoid-hash-operator]

// VIOLATION: Using # (stringize) operator
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// VIOLATION: Using ## (token paste) operator
#define CONCAT(a, b) a##b
#define MAKE_FUNC(name) void func_##name(void)

// VIOLATION: Multiple # or ## in single macro
#define DEFINE_VAR(type, name) type var_##name = 0; const char *name_##name = #name

// Problems with # and ##:
// 1. Order of evaluation is unspecified in some cases
// 2. Difficult to debug (happens at preprocessing)
// 3. Can create invalid tokens
// 4. Hard to read and maintain

void example(void) {
    // Using stringize
    const char *str = STRINGIFY(hello);  // "hello"
    (void)str;

    // Using token paste
    int CONCAT(my, var) = 42;  // Creates 'myvar'
    (void)myvar;

    // Creates func_init
    MAKE_FUNC(init);
}

// The generated name 'func_init' is hard to search for in code
void func_init(void) {}

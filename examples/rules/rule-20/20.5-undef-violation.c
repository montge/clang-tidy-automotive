// MISRA C:2025 Rule 20.5 - #undef usage
// Category: Advisory
// Status: VIOLATION
//
// #undef should not be used.
//
// Expected diagnostic:
//   warning: #undef should not be used [automotive-avoid-undef]

// VIOLATION: Using #undef
#define MAX_VALUE 100
// ... some code ...
#undef MAX_VALUE        // Undefining the macro
#define MAX_VALUE 200   // Redefining with different value

// VIOLATION: Undefining to redefine
#define BUFFER_SIZE 64
#undef BUFFER_SIZE
#define BUFFER_SIZE 128

// VIOLATION: Conditional undef
#ifdef DEBUG
#undef NDEBUG
#endif

// VIOLATION: Undefining standard macros
#undef NULL
#define NULL 0

// Problems with #undef:
// 1. Makes code harder to follow (macro has different values)
// 2. Can cause inconsistencies across translation units
// 3. Makes debugging more difficult
// 4. Can break code that depends on the original definition

void example(void) {
    int arr[MAX_VALUE];  // Which MAX_VALUE? 100 or 200?
    (void)arr;
}

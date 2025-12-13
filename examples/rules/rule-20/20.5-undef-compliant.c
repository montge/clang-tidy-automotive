// MISRA C:2025 Rule 20.5 - #undef usage
// Category: Advisory
// Status: COMPLIANT
//
// Macros are not undefined; different values use different names.

// COMPLIANT: Use different macro names for different values
#define MAX_VALUE_DEFAULT 100
#define MAX_VALUE_EXTENDED 200

// COMPLIANT: Use namespaced macros instead of redefining
#define MODULE_A_BUFFER_SIZE 64
#define MODULE_B_BUFFER_SIZE 128

// COMPLIANT: Use configuration macros selected at build time
#ifdef CONFIG_LARGE_BUFFERS
#define BUFFER_SIZE 256
#else
#define BUFFER_SIZE 64
#endif

// COMPLIANT: Use const variables instead of macros when possible
static const int MAX_ITEMS = 100;
static const int DEFAULT_TIMEOUT_MS = 1000;

// COMPLIANT: Use enums for related constants
typedef enum {
    SIZE_SMALL = 32,
    SIZE_MEDIUM = 64,
    SIZE_LARGE = 128,
    SIZE_XLARGE = 256
} BufferSize;

// COMPLIANT: Use inline functions instead of function-like macros
static inline int max_int(int a, int b) {
    return (a > b) ? a : b;
}

void example(void) {
    int arr_default[MAX_VALUE_DEFAULT];
    int arr_extended[MAX_VALUE_EXTENDED];
    int larger = max_int(10, 20);
    (void)arr_default;
    (void)arr_extended;
    (void)larger;
}

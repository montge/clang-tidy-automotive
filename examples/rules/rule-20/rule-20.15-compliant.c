// MISRA C:2025 Rule 20.15 Compliant Examples
// #define and #undef shall not be used on a reserved identifier or reserved macro name

// Compliant: Normal macro names
#define MY_MACRO 100
#define myMacro 200
#define VALID_NAME 300

// Compliant: Single underscore followed by lowercase is acceptable
// (not reserved in user code namespace)
#define _lowercase 400

// Compliant: Normal identifiers
#define MAX_BUFFER_SIZE 512
#define MIN_VALUE 1
#define DEFAULT_TIMEOUT 1000

// Compliant: Application-specific constants
#define APP_VERSION 1
#define CONFIG_ENABLED 1

void example_function(void) {
    int x = MY_MACRO;
    int y = VALID_NAME;
    int z = _lowercase;
    (void)x;
    (void)y;
    (void)z;
}

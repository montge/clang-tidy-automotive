// MISRA C:2025 Rule 8.2 - Function prototypes
// Category: Required
// Status: VIOLATION
//
// Function types shall be in prototype form with named parameters.
//
// Expected diagnostic:
//   warning: incomplete function prototype [automotive-uncomplete-function-prototype]

// VIOLATION: Empty parentheses (not a prototype in C)
int get_value();  // Means "unspecified parameters", not "no parameters"

// VIOLATION: Missing parameter names
void process(int, int);  // Parameters not named

// VIOLATION: K&R style function definition
int old_style_add(a, b)
    int a;
    int b;
{
    return a + b;
}

// VIOLATION: Function pointer without prototype
typedef int (*callback)();  // Unspecified parameters

// VIOLATION: Mixed - some parameters named, some not
void mixed_params(int x, float, char c);

// Note: In C, empty () means unspecified parameters.
// Use (void) to explicitly indicate no parameters.

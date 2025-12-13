// MISRA C:2025 Rule 20.4 - Macro named as keyword
// Category: Required
// Status: VIOLATION
//
// A macro shall not be defined with the same name as a keyword.
//
// Expected diagnostic:
//   warning: macro named as C keyword [automotive-avoid-macro-named-as-ckeyword]

// VIOLATION: Redefining C keywords as macros
#define if while        // Redefines 'if'
#define int long        // Redefines 'int'
#define return goto     // Redefines 'return'
#define void char       // Redefines 'void'
#define const           // Undefines 'const' behavior
#define static extern   // Redefines 'static'

// VIOLATION: Redefining bool/true/false (C23 keywords)
#define true 0          // Swaps true/false
#define false 1
#define bool int

// VIOLATION: Redefining sizeof
#define sizeof(x) 4     // Breaks sizeof operator

// These redefinitions can cause severe confusion and
// make code behave unexpectedly. They may also break
// standard library headers that expect normal behavior.

void example(void) {
    // With above macros, this code does something unexpected
    // if (x) becomes while (x)
    // return 0; becomes goto 0; (syntax error)
}

// RUN: %check_clang_tidy %s automotive-c23-req-5.4 %t

// Test: Macro identifiers shall be distinct from other identifiers (MISRA Rule 5.4)

// Declare a variable first
int counter = 0;

// Violation - macro name conflicts with variable
#define counter 100
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: macro 'counter' conflicts with variable identifier

// Declare a function
void process(int x);

// Violation - macro name conflicts with function
#define process(x) ((x) * 2)
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: macro 'process' conflicts with function identifier

// Declare a typedef
typedef int MyInt;

// Violation - macro name conflicts with typedef
#define MyInt long
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: macro 'MyInt' conflicts with typedef identifier

// Compliant - unique macro name
#define UNIQUE_MACRO 42

// Compliant - unique variable name
int unique_var = 0;

// Compliant - macro defined before any identifier with same name
#define BUFFER_SIZE 1024
int buffer[BUFFER_SIZE];

// Use declarations to avoid unused warnings
void use_declarations(void) {
    (void)counter;
    (void)unique_var;
    (void)buffer[0];
    process(1);
    MyInt x = 0;
    (void)x;
}

void process(int x) {
    (void)x;
}

// RUN: %check_clang_tidy %s automotive-c23-req-5.4 %t

// Test: Macro identifiers shall be distinct from other identifiers (MISRA Rule 5.4)

// Declare a variable first
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: variable 'counter' conflicts with macro identifier [automotive-c23-req-5.4]
int counter = 0;

// Violation - macro name conflicts with variable
// CHECK-MESSAGES: :[[@LINE+1]]:9: note: macro 'counter' defined here
#define counter 100

// Declare a function
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'process' conflicts with macro identifier [automotive-c23-req-5.4]
void process(int x);

// Violation - macro name conflicts with function
// CHECK-MESSAGES: :[[@LINE+1]]:9: note: macro 'process' defined here
#define process(x) ((x) * 2)

// Declare a typedef
// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: typedef 'MyInt' conflicts with macro identifier [automotive-c23-req-5.4]
typedef int MyInt;

// Violation - macro name conflicts with typedef
// CHECK-MESSAGES: :[[@LINE+1]]:9: note: macro 'MyInt' defined here
#define MyInt long

// Compliant - unique macro name
#define UNIQUE_MACRO 42

// Compliant - unique variable name
int unique_var = 0;

// Compliant - macro defined before any identifier with same name
#define BUFFER_SIZE 1024
int buffer[BUFFER_SIZE];

// Use declarations to avoid unused warnings
void use_declarations(void) {
    (void)unique_var;
    (void)buffer[0];
}

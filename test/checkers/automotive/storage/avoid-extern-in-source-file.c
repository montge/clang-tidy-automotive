// RUN: %check_clang_tidy %s automotive-c25-adv-8.19 %t

// MISRA C:2025 Rule 8.19: There should be no external declarations in a source file

// Non-compliant: extern variable declaration in source file
extern int global_from_other_file;
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: external variable declaration 'global_from_other_file' should be in a header file, not a source file

// Non-compliant: extern function declaration in source file
extern void external_function(void);
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: external function declaration 'external_function' should be in a header file

// Non-compliant: extern with explicit keyword
extern int another_extern_var;
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: external variable declaration 'another_extern_var' should be in a header file, not a source file

// Compliant: function definition (not a declaration)
void local_function(void) {
  // OK - this is a definition, not an extern declaration
}

// Compliant: static function declaration
static void internal_function(void);

// Compliant: static variable
static int internal_var;

// Compliant: file-scope variable without extern (definition)
int file_scope_var = 42;

// Compliant: local variables (even with extern semantics)
void test_function(void) {
  int local = 1;  // OK - local variable
  (void)local;
}

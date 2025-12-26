// RUN: %check_clang_tidy %s automotive-c23-req-21.2 %t -- -- -std=c11
// Test for automotive-c23-req-21.2: reserved identifiers shall not be declared

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: variable declaration uses reserved identifier '__double_underscore' [automotive-c23-req-21.2]
int __double_underscore = 0;

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: variable declaration uses reserved identifier '_Uppercase_letter' [automotive-c23-req-21.2]
int _Uppercase_letter = 0;

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: struct declaration uses reserved identifier '_ReservedStruct' [automotive-c23-req-21.2]
struct _ReservedStruct {
  int value;
};

// OK - underscore followed by lowercase (not reserved in file scope)
int _lowercase = 0;

// OK - normal identifiers
static int my_variable = 0;
static int myVariable = 0;
static int CONSTANT = 0;
static int value_1 = 0;

static void my_function(void) {
  // OK - local variables with normal names
  int local_var = 0;
  (void)local_var;
}

// OK - macro names don't start with underscore
#define MY_MACRO 42

// OK - typedef with normal name
typedef int my_int_t;

int main(void) {
  (void)__double_underscore;
  (void)_Uppercase_letter;
  (void)_lowercase;
  (void)my_variable;
  (void)myVariable;
  (void)CONSTANT;
  (void)value_1;
  my_function();
  (void)MY_MACRO;
  my_int_t x = 0;
  (void)x;
  return 0;
}

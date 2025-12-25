// RUN: %check_clang_tidy %s automotive-c25-req-5.2 %t -- -- -std=c11
// Test for automotive-c25-req-5.2: reserved identifiers shall not be declared

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: reserved identifier declared
int _reserved_underscore;

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: reserved identifier declared
int __double_underscore;

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: reserved identifier declared
int _Uppercase_letter;

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: reserved identifier declared
void _my_function(void) {
}

struct _ReservedStruct {
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: reserved identifier declared
  int value;
};

// OK - normal identifiers
int my_variable;
int myVariable;
int CONSTANT;
int value_1;

void my_function(void) {
  // OK - local variables with normal names
  int local_var;
  int _local_underscore;  // Local scope may be OK depending on interpretation
  (void)local_var;
  (void)_local_underscore;
}

// OK - macro names don't start with underscore
#define MY_MACRO 42

// OK - typedef with normal name
typedef int my_int_t;

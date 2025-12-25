// RUN: %check_clang_tidy %s automotive-c25-adv-17.12 %t

// Test MISRA C:2025 Rule 17.12 (Advisory):
// A function identifier should only be used with & or parenthesized parameter list.

void test_function(void);
void another_function(int x);
int returns_int(void);

void compliant_examples(void) {
  void (*ptr1)(void);
  void (*ptr2)(int);
  int (*ptr3)(void);

  // Compliant: Using & operator
  ptr1 = &test_function;
  ptr2 = &another_function;
  ptr3 = &returns_int;

  // Compliant: Function call with ()
  test_function();
  another_function(5);
  int result = returns_int();

  // Compliant: Using & in initialization
  void (*ptr4)(void) = &test_function;
  void (*ptr5)(int) = &another_function;
}

void non_compliant_examples(void) {
  void (*ptr1)(void);
  void (*ptr2)(int);
  int (*ptr3)(void);

  // Non-compliant: Direct assignment without &
  ptr1 = test_function;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  ptr2 = another_function;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function identifier 'another_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  ptr3 = returns_int;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function identifier 'returns_int' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  // Non-compliant: Initialization without &
  void (*ptr4)(void) = test_function;
  // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  void (*ptr5)(int) = another_function;
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: function identifier 'another_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]
}

// Non-compliant: Function passed as argument without &
void callback_test(void (*callback)(void)) {
  // body
}

void test_callback_passing(void) {
  callback_test(test_function);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  // Compliant: Using &
  callback_test(&test_function);
}

// Non-compliant: Return function pointer without &
void (*get_function_ptr(void))(void) {
  return test_function;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]
}

// Compliant: Return with &
void (*get_function_ptr_compliant(void))(void) {
  return &test_function;
}

// Non-compliant: Array of function pointers initialization
void (*func_ptr_array[2])(void) = {
  test_function,
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]
  &test_function  // Compliant: has &
};

// Non-compliant: Conditional expression
void test_conditional(int flag) {
  void (*ptr)(void);
  void (*ptr2)(void);
  ptr = flag ? test_function : ptr2;
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  // Compliant: with &
  ptr = flag ? &test_function : &test_function;
}

// Non-compliant: Comparison (though unusual)
void test_comparison(void) {
  void (*ptr)(void) = &test_function;

  if (ptr == test_function) {  // Non-compliant
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]
    // do something
  }

  if (ptr == &test_function) {  // Compliant
    // do something
  }
}

// Struct with function pointer member
struct callback_struct {
  void (*callback)(void);
};

void test_struct_init(void) {
  // Non-compliant: Direct initialization
  struct callback_struct s1 = { test_function };
  // CHECK-MESSAGES: :[[@LINE-1]]:33: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  // Compliant: With &
  struct callback_struct s2 = { &test_function };

  // Non-compliant: Member assignment
  struct callback_struct s3;
  s3.callback = test_function;
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function identifier 'test_function' should only be used with & or a parenthesized parameter list [automotive-c25-adv-17.12]

  // Compliant: Member assignment with &
  s3.callback = &test_function;
}

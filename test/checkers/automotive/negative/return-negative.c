// RUN: clang-tidy %s --checks='-*,automotive-missing-return-void,automotive-missing-return-value-handling' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger return warnings.

// Void function - no return needed
void void_function(void) {
  int x = 10;
  (void)x;
  return;  // explicit return is fine for void
}

// Non-void function with proper return
int get_value(void) {
  return 42;
}

// Single return point
int process_data(int x) {
  int result = x * 2;
  return result;
}

// Properly handling return values
void use_return_values(void) {
  int val = get_value();  // return value used
  int processed = process_data(10);  // return value used
  (void)val;
  (void)processed;
}

// Functions that should have void return
void perform_action(int action) {
  switch (action) {
    case 1:
      break;
    case 2:
      break;
    default:
      break;
  }
}

// All paths return in non-void function
int conditional_return(int x) {
  if (x > 0) {
    return 1;
  } else {
    return -1;
  }
}

// CHECK-NOT: warning:

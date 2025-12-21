// RUN: clang-tidy %s --checks='-*,automotive-c23-req-9.1,automotive-c23-req-21.2' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger storage warnings.

// Properly initialized variables
static void init_variables(void) {
  int a = 0;
  int b = 10;
  float f = 0.0f;
  char c = 'x';

  a = b + 1;
  f = f + 1.0f;
  (void)c;
  (void)a;
  (void)f;
}

// Static for internal linkage - proper usage
static int internal_counter = 0;

static void internal_function(void) {
  internal_counter++;
}

// Proper identifier names (not reserved)
static int my_variable = 0;
static void my_function(void) {
  int local_var = 0;
  (void)local_var;
}

// Properly scoped variables
static void scoped_variables(void) {
  for (int i = 0; i < 10; i++) {
    int temp = i * 2;
    (void)temp;
  }

  {
    int block_var = 100;
    (void)block_var;
  }
}

// Use internal items
void use_internals(void) {
  internal_function();
  my_function();
  init_variables();
  scoped_variables();
  (void)internal_counter;
  (void)my_variable;
}

// CHECK-NOT: warning:

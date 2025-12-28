// RUN: %check_clang_tidy %s automotive-c25-req-2.2 %t

// MISRA C:2025 Rule 2.2: Dead code shall not exist

void test_dead_code() {
  int x = 5;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dead code: operation has no effect [automotive-c25-req-2.2]
  x + 10;  // Expression has no effect

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dead code: literal has no effect [automotive-c25-req-2.2]
  42;      // Literal has no effect

  int y;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dead code: expression has no effect [automotive-c25-req-2.2]
  y;       // Variable reference has no effect

  // Valid code - has side effect
  x = 20;  // Assignment - OK

  // Valid code - function call
  extern void func(void);
  func();  // OK - function call

  // Valid code - increment
  x++;     // OK - has side effect

  // Valid code - used in condition
  if (x > 0) {
    x = 0;
  }
}

void test_dead_operations() {
  int a = 1, b = 2;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dead code: operation has no effect [automotive-c25-req-2.2]
  a * b;   // Multiplication with no effect

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dead code: operation has no effect [automotive-c25-req-2.2]
  a - b;   // Subtraction with no effect

  // Valid - assignment
  a = a * b;  // OK

  // Valid - compound assignment
  a *= b;     // OK
}

void test_pointer_operations() {
  int x = 0;
  int *p = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dead code: expression has no effect [automotive-c25-req-2.2]
  *p;      // Dereference with no effect

  // Valid - assignment through pointer
  *p = 5;  // OK
}

void test_valid_expressions() {
  int x = 0;

  // These should NOT trigger warnings
  x = x + 1;           // OK - assignment
  x += 5;              // OK - compound assignment
  ++x;                 // OK - increment
  --x;                 // OK - decrement

  extern int get_value(void);
  x = get_value();     // OK - function call with assignment

  extern void process(int);
  process(x);          // OK - function call

  // OK - used in return
  int y = x * 2;
}

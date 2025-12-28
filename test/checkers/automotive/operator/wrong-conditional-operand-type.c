// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-mand-7.5 %t

int *global_ptr;
int global_int;

void test_conditional_operator(int cond) {
  int *p = &global_int;
  int x = 42;

  // Violations: mixing pointer and integer (non-NULL)
  int *r1 = cond ? p : 5;
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: incompatible pointer and integer types used as operands to conditional operator

  int *r2 = cond ? 10 : p;
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: incompatible pointer and integer types used as operands to conditional operator

  void *r3 = cond ? p : x;
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: incompatible pointer and integer types used as operands to conditional operator

  // Allowed: NULL pointer constant (integer 0)
  int *r4 = cond ? p : 0;      // OK
  int *r5 = cond ? 0 : p;      // OK
  int *r6 = cond ? p : ((void*)0);  // OK

  // Allowed: both operands are pointers
  int *r7 = cond ? p : global_ptr;  // OK

  // Allowed: both operands are integers
  int r8 = cond ? x : 5;  // OK

  // Allowed: both are the same type
  int r9 = cond ? 1 : 2;  // OK
}

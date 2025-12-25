// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c11
// Test for automotive-missing-compound: control statements shall use compound statements

void test_if_without_braces(int x) {
  // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: missing compound statement
  if (x > 0)
    x++;
}

void test_else_without_braces(int x) {
  if (x > 0) {
    x++;
  // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: missing compound statement
  } else
    x--;
}

void test_while_without_braces(int x) {
  // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: missing compound statement
  while (x > 0)
    x--;
}

void test_for_without_braces(int n) {
  int sum = 0;
  // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: missing compound statement
  for (int i = 0; i < n; i++)
    sum += i;
  (void)sum;
}

void test_do_without_braces(int x) {
  // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: missing compound statement
  do
    x--;
  while (x > 0);
}

void test_if_with_braces_compliant(int x) {
  // OK - uses compound statement
  if (x > 0) {
    x++;
  }
}

void test_while_with_braces_compliant(int x) {
  // OK - uses compound statement
  while (x > 0) {
    x--;
  }
}

void test_for_with_braces_compliant(int n) {
  // OK - uses compound statement
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += i;
  }
  (void)sum;
}

void test_do_with_braces_compliant(int x) {
  // OK - uses compound statement
  do {
    x--;
  } while (x > 0);
}

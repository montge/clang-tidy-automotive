// RUN: %check_clang_tidy %s automotive-avoid-boolean-in-switch %t -- -- -std=c11
// Test for automotive-avoid-boolean-in-switch: switch shall not use boolean expression

#include <stdbool.h>

void test_boolean_switch_violation(bool flag) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: avoid boolean expression in switch statement
  switch (flag) {
  case true:
    break;
  case false:
    break;
  }
}

void test_boolean_expr_switch_violation(int a, int b) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: avoid boolean expression in switch statement
  switch (a == b) {
  case 1:
    break;
  case 0:
    break;
  }
}

void test_int_switch_compliant(int value) {
  // OK - integer expression in switch
  switch (value) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    break;
  default:
    break;
  }
}

enum Color { RED, GREEN, BLUE };

void test_enum_switch_compliant(enum Color c) {
  // OK - enum expression in switch
  switch (c) {
  case RED:
    break;
  case GREEN:
    break;
  case BLUE:
    break;
  }
}

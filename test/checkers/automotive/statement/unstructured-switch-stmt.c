// RUN: %check_clang_tidy %s automotive-x-req-16.2 %t -- -- -std=c11
// Test for automotive-x-req-16.2: switch cases shall be at top level of switch body

void test_nested_case_violation(int x, int y) {
  switch (x) {
  case 1:
    if (y > 0) {
      // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: case/default label not at switch body top level
      case 2:
        break;
    }
    break;
  case 3:
    break;
  }
}

void test_case_in_loop_violation(int x) {
  switch (x) {
  case 1:
    for (int i = 0; i < 10; i++) {
      // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: case/default label not at switch body top level
      case 2:
        break;
    }
    break;
  }
}

void test_default_nested_violation(int x) {
  switch (x) {
  case 1:
    {
      // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: case/default label not at switch body top level
      default:
        break;
    }
    break;
  }
}

void test_structured_switch_compliant(int x) {
  // OK - all cases at top level
  switch (x) {
  case 1:
    // action 1
    break;
  case 2:
    // action 2
    break;
  case 3:
    // action 3
    break;
  default:
    // default action
    break;
  }
}

void test_nested_switch_compliant(int x, int y) {
  // OK - nested switch, but each switch has top-level cases
  switch (x) {
  case 1:
    switch (y) {
    case 1:
      break;
    case 2:
      break;
    }
    break;
  case 2:
    break;
  }
}

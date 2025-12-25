// RUN: %check_clang_tidy %s automotive-missing-else %t -- -- -std=c11
// Test for automotive-missing-else: if...else if chain shall terminate with else

void test_missing_else_violation(int x) {
  if (x == 1) {
    // action 1
  } else if (x == 2) {
    // action 2
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: if...else if chain should terminate with an else clause
  }
}

void test_nested_missing_else_violation(int x, int y) {
  if (x == 1) {
    // action 1
  } else if (x == 2) {
    // action 2
  } else if (x == 3) {
    // action 3
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: if...else if chain should terminate with an else clause
  }
}

void test_complete_else_if_chain(int x) {
  // OK - chain terminates with else
  if (x == 1) {
    // action 1
  } else if (x == 2) {
    // action 2
  } else {
    // default action
  }
}

void test_simple_if_compliant(int x) {
  // OK - simple if without else if
  if (x == 1) {
    // action
  }
}

void test_simple_if_else_compliant(int x) {
  // OK - simple if-else
  if (x == 1) {
    // action 1
  } else {
    // action 2
  }
}

void test_multiple_complete_chains(int x, int y) {
  // OK - first chain complete
  if (x == 1) {
    // action 1
  } else if (x == 2) {
    // action 2
  } else {
    // default
  }

  // OK - second chain complete
  if (y == 1) {
    // action 1
  } else {
    // default
  }
}

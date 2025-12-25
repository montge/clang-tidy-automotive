// RUN: %check_clang_tidy %s automotive-c23-req-16.5 %t -- -- -std=c11
// Test for automotive-c23-req-16.5: default clause shall be first or last in switch

void test_default_in_middle_violation(int x) {
  switch (x) {
  case 1:
    break;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: default statement neither first or last
  default:
    break;
  case 2:
    break;
  }
}

void test_default_first_compliant(int x) {
  // OK - default is first
  switch (x) {
  default:
    break;
  case 1:
    break;
  case 2:
    break;
  }
}

void test_default_last_compliant(int x) {
  // OK - default is last
  switch (x) {
  case 1:
    break;
  case 2:
    break;
  default:
    break;
  }
}

void test_no_default_compliant(int x) {
  // OK - no default clause
  switch (x) {
  case 1:
    break;
  case 2:
    break;
  }
}

void test_only_default_compliant(int x) {
  // OK - only default clause
  switch (x) {
  default:
    break;
  }
}

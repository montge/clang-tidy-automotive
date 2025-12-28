// RUN: %check_clang_tidy %s automotive-c23-req-8.18 %t
// Test for automotive-c23-req-8.18
// Related MISRA C:2025 Rule: 8.18

// This test verifies that tentative definitions in header files are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: tentative definition of 'tentative_var' in header file; use 'extern' for declaration or provide an initializer [automotive-c23-req-8.18]
int tentative_var;

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: tentative definition of 'tentative_array' in header file; use 'extern' for declaration or provide an initializer [automotive-c23-req-8.18]
char tentative_array[100];

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Explicit initialization is compliant
int initialized_var = 0;
char initialized_array[10] = {0};

// extern declaration is compliant
extern int declared_var;
extern char declared_array[];

// static in header (different rule, but not a tentative definition issue)
static int static_var;

// const with initialization
const int const_var = 42;

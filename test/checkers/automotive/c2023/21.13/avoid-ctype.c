// RUN: %check_clang_tidy %s automotive-c23-mand-21.13 %t -- -- -std=c99

// MISRA C:2025 Rule 21.13 (Mandatory)
// The values passed to ctype.h functions shall be representable as
// an unsigned char or be the value EOF.

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inclusion of <ctype.h> is not allowed in safety-critical code [automotive-c23-mand-21.13]
#include <ctype.h>

void example() {
  char c = 'a';
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: use of 'isdigit' is not allowed in safety-critical code [automotive-c23-mand-21.13]
  if (isdigit(c)) {
    // do something
  }
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: use of 'isalpha' is not allowed in safety-critical code [automotive-c23-mand-21.13]
  if (isalpha(c)) {
    // do something
  }
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: use of 'toupper' is not allowed in safety-critical code [automotive-c23-mand-21.13]
  c = toupper(c);
  (void)c;
}

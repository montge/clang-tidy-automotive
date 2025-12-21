// RUN: %check_clang_tidy %s automotive-x-adv-2.4 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-x-adv-2.4 %t -- -- -std=c11

// MISRA C:2025 Rule 2.4 (Advisory)
// A project should not contain unused tag declarations.

enum color2 { RED2, GREEN2, BLUE2 }; /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: unused tag 'color2' [automotive-x-adv-2.4]

void f_enum(void) {
  enum color { RED, GREEN, BLUE }; /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: unused tag 'color' [automotive-x-adv-2.4]

  enum status { OK, FAIL };        /* Not compliant - tag is unused even though typedef is used */
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: unused tag 'status' [automotive-x-adv-2.4]

  enum status test = OK;

  typedef enum ready {             /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: unused tag 'ready' [automotive-x-adv-2.4]
    INIT,
    READY,
    WAIT
  } ready_t;

  ready_t r = INIT;
}

void f(enum color2 *c) {
}

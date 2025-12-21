// RUN: %check_clang_tidy %s automotive-unused-macro %t

#define SIZE 4  /* Compliant */
#define DATA 3  /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: unused macro definition 'DATA' [automotive-unused-macro]

int a[SIZE];

#define TEMP 10  /* Compliant - This is due to the undef */
#undef TEMP

#define TEST 42  /* Not compliant - Not used after redefined. */
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: unused macro definition 'TEST' [automotive-unused-macro]

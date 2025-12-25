// RUN: %check_clang_tidy %s automotive-c25-req-4.2 %t -- -- -std=c11 -trigraphs
// Test for automotive-c25-req-4.2: trigraph sequences shall not be used

// Note: This test requires -trigraphs flag to enable trigraph support in Clang

// The following would be trigraph violations if trigraphs were enabled:
// ??= expands to #
// ??( expands to [
// ??) expands to ]
// ??< expands to {
// ??> expands to }
// ??/ expands to \
// ??' expands to ^
// ??! expands to |
// ??- expands to ~

// CHECK-MESSAGES: :[[@LINE+1]]:14: warning: avoid trigraph sequence '??=' (expands to '#')
char *test = "??=";  // Contains ??= trigraph

// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: avoid trigraph sequence '??(' (expands to '[')
char *bracket1 = "??(";

// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: avoid trigraph sequence '??)' (expands to ']')
char *bracket2 = "??)";

void test_compliant(void) {
  // OK - no trigraph sequences
  char *str = "Hello World";
  char *question = "??";  // Just two question marks, not a trigraph
  (void)str;
  (void)question;
}

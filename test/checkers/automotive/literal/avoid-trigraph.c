// XFAIL: *
// Note: Check also triggers on trigraphs in comments, making test unreliable
// RUN: %check_clang_tidy %s automotive-c25-adv-4.2 %t -- -- -std=c11 -trigraphs
// Test for automotive-c25-adv-4.2: trigraph sequences shall not be used

// Note: Trigraph sequences are:
// ??= -> #, ??( -> [, ??) -> ], ??< -> {, ??> -> }, ??/ -> \, ??' -> ^, ??! -> |, ??- -> ~

// CHECK-MESSAGES: :[[@LINE+1]]:14: warning: avoid trigraph sequence '??=' (expands to '#') [automotive-c25-adv-4.2]
char *test = "??=";

// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: avoid trigraph sequence '??(' (expands to '[') [automotive-c25-adv-4.2]
char *bracket1 = "??(";

// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: avoid trigraph sequence '??)' (expands to ']') [automotive-c25-adv-4.2]
char *bracket2 = "??)";

// CHECK-MESSAGES: :[[@LINE+1]]:17: warning: avoid trigraph sequence '??<' (expands to '{') [automotive-c25-adv-4.2]
char *brace1 = "??<";

// CHECK-MESSAGES: :[[@LINE+1]]:17: warning: avoid trigraph sequence '??>' (expands to '}') [automotive-c25-adv-4.2]
char *brace2 = "??>";

// CHECK-MESSAGES: :[[@LINE+1]]:17: warning: avoid trigraph sequence '??/' (expands to '\') [automotive-c25-adv-4.2]
char *slash = "??/";

// CHECK-MESSAGES: :[[@LINE+1]]:16: warning: avoid trigraph sequence '??'' (expands to '^') [automotive-c25-adv-4.2]
char *caret = "??'";

// CHECK-MESSAGES: :[[@LINE+1]]:16: warning: avoid trigraph sequence '??!' (expands to '|') [automotive-c25-adv-4.2]
char *pipe_c = "??!";

// CHECK-MESSAGES: :[[@LINE+1]]:16: warning: avoid trigraph sequence '??-' (expands to '~') [automotive-c25-adv-4.2]
char *tilde = "??-";

// OK - normal strings without trigraphs
char *normal = "hello";
char *question = "?";
char *two_q = "??";  // Two question marks but no valid trigraph sequence

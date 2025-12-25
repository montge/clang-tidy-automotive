// RUN: %check_clang_tidy %s automotive-c25-req-1.5 %t -- -- -std=c11
// Test for automotive-c25-req-1.5: obsolescent language features

// CHECK-MESSAGES: :[[@LINE+2]]:5: warning: obsolescent feature: function 'old_style' uses old-style declaration
// K&R style declaration - obsolescent
int old_style();

// OK - modern prototype style
int modern_style(int x, int y);

// OK - void parameter list
int no_params(void);

int main(void) {
  return 0;
}

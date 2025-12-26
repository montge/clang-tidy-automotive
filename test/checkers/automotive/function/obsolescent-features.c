// RUN: %check_clang_tidy %s automotive-uncomplete-function-prototype %t -- -- -std=c89
// Test for automotive-uncomplete-function-prototype: function not in prototype form

// K&R style function definition - obsolescent
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function is not in prototype form [automotive-uncomplete-function-prototype]
int old_style(x, y)
int x;
int y;
{
  return x + y;
}

// OK - modern prototype style function definition
int modern_style(int x, int y) {
  return x + y;
}

// OK - void parameter list
int no_params(void) {
  return 0;
}

int main(void) {
  return 0;
}

// RUN: %check_clang_tidy %s automotive-avoid-undef %t -- -- -I %S

#include <stdio.h>

#define HEADER2 "header2.h"
#include HEADER2

int main(void)
{
  return 0;
}

#define PASS 0
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef PASS


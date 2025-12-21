// RUN: clang-tidy %s --checks='-*,automotive-wrong-bitfield-type,automotive-avoid-signed-single-bitfield,automotive-avoid-bitfield-in-union' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger bitfield warnings.

#include <stdbool.h>

// Properly typed bitfields
struct valid_bitfields {
  unsigned int flag1 : 1;   // unsigned single-bit - compliant
  unsigned int flag2 : 1;   // unsigned single-bit - compliant
  unsigned int value : 4;   // unsigned multi-bit - compliant
  signed int svalue : 4;    // signed multi-bit - compliant
  _Bool bflag : 1;          // _Bool single-bit - compliant
};

// Struct with proper types
struct control_register {
  unsigned int enable : 1;
  unsigned int mode : 3;
  unsigned int status : 4;
  unsigned int reserved : 24;
};

// No bitfields in unions (compliant usage)
union data_union {
  int as_int;
  float as_float;
  char as_bytes[4];
};

// Regular struct with no issues
struct regular_struct {
  int x;
  int y;
  char name[32];
};

// Using the structs
void use_bitfields(void) {
  struct valid_bitfields bf = {0};
  bf.flag1 = 1;
  bf.value = 10;
  (void)bf;
}

// CHECK-NOT: warning:

// RUN: %check_clang_tidy %s automotive-c25-req-6.1 %t -- -- -std=c11
// Test for automotive-c25-req-6.1: signed single-bit bitfields shall not be used

struct SingleBitSigned {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: signed single-bit bitfield
  int flag : 1;  // Signed single-bit - violation
};

struct SingleBitSignedExplicit {
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: signed single-bit bitfield
  signed int bit : 1;  // Explicitly signed single-bit - violation
};

struct MultipleSingleBits {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: signed single-bit bitfield
  int a : 1;
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: signed single-bit bitfield
  int b : 1;
  unsigned int c : 1;  // OK - unsigned
};

struct UnsignedSingleBit {
  unsigned int flag : 1;  // OK - unsigned single-bit
};

struct MultiBitSigned {
  int value : 4;  // OK - multi-bit signed is allowed
};

struct BoolBitfield {
  _Bool flag : 1;  // OK - _Bool is inherently unsigned
};

struct MixedBitfields {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: signed single-bit bitfield
  int sign_flag : 1;    // Violation
  unsigned int data : 8;  // OK
  int counter : 4;        // OK - multi-bit
};

struct NestedStruct {
  struct {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: signed single-bit bitfield
    int nested_bit : 1;  // Violation in nested struct
  } inner;
};

// Test file for: automotive-avoid-signed-single-bitfield
// Related MISRA C:2025 Rule: 6.2
//
// This file tests the detection of single-bit bit-fields with signed types

// RUN: %check_clang_tidy %s automotive-avoid-signed-single-bitfield %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

struct BadStruct1 {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: signed single-bit bit-field
    int flag : 1;
};

struct BadStruct2 {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: signed single-bit bit-field
    signed int a : 1;
    unsigned int b : 1;  // OK - unsigned
};

struct BadStruct3 {
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: signed single-bit bit-field
    char bit : 1;  // char may be signed by default
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Unsigned single-bit - compliant
struct GoodStruct1 {
    unsigned int flag : 1;
};

// Multi-bit signed field - compliant
struct GoodStruct2 {
    int value : 4;  // Multiple bits, signed is OK
};

// Unsigned char single-bit - compliant
struct GoodStruct3 {
    unsigned char bit : 1;
};

// _Bool single-bit - compliant
struct GoodStruct4 {
    _Bool flag : 1;
};

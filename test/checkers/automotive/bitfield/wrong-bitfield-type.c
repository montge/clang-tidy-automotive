// Test file for: automotive-wrong-bitfield-type
// Related MISRA C:2025 Rule: 6.1
//
// This file tests the detection of bit-fields with inappropriate types

// RUN: %check_clang_tidy %s automotive-wrong-bitfield-type %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

enum Color { RED, GREEN, BLUE };

struct BadStruct1 {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: inappropriate bit-field type
    enum Color color : 2;  // Enum as bit-field type
};

// Note: float/double as bit-field would be a compile error, not detectable

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Unsigned int - compliant
struct GoodStruct1 {
    unsigned int flags : 4;
};

// Signed int - compliant
struct GoodStruct2 {
    int value : 8;
};

// _Bool - compliant
struct GoodStruct3 {
    _Bool flag : 1;
};

// unsigned char - compliant
struct GoodStruct4 {
    unsigned char bits : 4;
};

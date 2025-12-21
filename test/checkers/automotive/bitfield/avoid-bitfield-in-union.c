// Test file for: automotive-avoid-bitfield-in-union
// Related MISRA C:2025 Rule: 19.2
//
// This file tests the detection of bit-field members within union declarations

// RUN: %check_clang_tidy %s automotive-avoid-bitfield-in-union %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Basic violation: bit-field in union
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: avoid bitfield in union
union BadUnion1 {
    unsigned int flags : 8;
    int value;
};

// Multiple bit-fields in union
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: avoid bitfield in union
union BadUnion2 {
    unsigned int a : 4;
    unsigned int b : 4;
};

// Nested structure with bit-field (union contains struct, not direct bit-field)
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: avoid bitfield in union
union BadUnion3 {
    struct {
        unsigned int bits : 16;  // This is in struct, not directly in union
    } s;
    int data : 8;  // Direct bit-field in union
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Union without bit-fields - compliant
union GoodUnion1 {
    unsigned int flags;
    int value;
    float f;
};

// Struct with bit-fields - compliant (not a union)
struct GoodStruct {
    unsigned int flags : 8;
    unsigned int data : 24;
};

// Empty union - compliant
union EmptyUnion {
    int placeholder;
};

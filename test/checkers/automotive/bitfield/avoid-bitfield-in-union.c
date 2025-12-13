// Test file for: automotive-avoid-bitfield-in-union
// Related MISRA C:2025 Rule: 19.2
//
// This file tests the detection of bit-field members within union declarations

// RUN: %check_clang_tidy %s automotive-avoid-bitfield-in-union %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Basic violation: bit-field in union
union BadUnion1 {
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bit-field in union
    unsigned int flags : 8;
    int value;
};

// Multiple bit-fields in union
union BadUnion2 {
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bit-field in union
    unsigned int a : 4;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bit-field in union
    unsigned int b : 4;
};

// Nested structure with bit-field (union contains struct, not direct bit-field)
union BadUnion3 {
    struct {
        unsigned int bits : 16;  // This is in struct, not directly in union
    } s;
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: bit-field in union
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

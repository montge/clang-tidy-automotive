// Test file for: automotive-avoid-union
// Related MISRA C:2025 Rule: 19.2
//
// This file tests the detection of union type usage

// RUN: %check_clang_tidy %s automotive-avoid-union %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: avoid union keyword
union SimpleUnion {
    int i;
    float f;
};

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: avoid union keyword
union DataUnion {
    char bytes[4];
    int value;
};

// Anonymous union in struct (not detected by check - see check implementation)
struct Container {
    int type;
    union {
        int i;
        float f;
    } data;
};

// Typedef'd union
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid union keyword
typedef union {
    unsigned char bytes[8];
    double value;
} ByteDouble;

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Struct is fine
struct GoodStruct {
    int i;
    float f;
};

// Nested structs
struct Container2 {
    int type;
    struct {
        int i;
        float f;
    } data;
};

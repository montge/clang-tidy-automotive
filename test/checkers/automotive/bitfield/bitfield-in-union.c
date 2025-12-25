// Test file for: automotive-c25-req-6.3
// MISRA C:2025 Rule 6.3 (Required)
//
// This file tests the detection of bit-field declarations as members of unions

// RUN: %check_clang_tidy %s automotive-c25-req-6.3 %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Basic violation: single bit-field in union
union BadUnion1 {
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bit-field shall not be declared as a member of a union
    unsigned int flags : 8;
    int value;
};

// Multiple bit-fields in union
union BadUnion2 {
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bit-field shall not be declared as a member of a union
    unsigned int a : 4;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bit-field shall not be declared as a member of a union
    unsigned int b : 4;
};

// Mixed bit-field and regular members
union BadUnion3 {
    int regular_field;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bit-field shall not be declared as a member of a union
    unsigned int bits : 16;
};

// Signed bit-field in union
union BadUnion4 {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: bit-field shall not be declared as a member of a union
    int sign : 1;
    unsigned int value;
};

// Bool bit-field in union
union BadUnion5 {
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: bit-field shall not be declared as a member of a union
    _Bool flag : 1;
    int data;
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
struct GoodStruct1 {
    unsigned int flags : 8;
    unsigned int data : 24;
};

// Empty union - compliant
union EmptyUnion {
    int placeholder;
};

// Union with nested struct containing bit-field - compliant
// (bit-field is in struct, not directly in union)
union GoodUnion2 {
    struct {
        unsigned int bits : 16;
    } s;
    int data;
};

// Regular fields only
union GoodUnion3 {
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

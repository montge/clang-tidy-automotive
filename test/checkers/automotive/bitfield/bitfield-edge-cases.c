// Test file for: automotive bitfield checks (Edge Cases)
// Related MISRA C:2025 Rules: 6.1, 6.2, 19.2
//
// This file tests edge cases for bitfield-related checks

// RUN: %check_clang_tidy %s automotive-wrong-bitfield-type,automotive-avoid-signed-single-bitfield,automotive-avoid-bitfield-in-union %t

#include <stdbool.h>

//===----------------------------------------------------------------------===//
// Edge Case: Zero-width bitfield (anonymous)
//===----------------------------------------------------------------------===//

struct ZeroWidth {
    unsigned int a : 4;
    unsigned int : 0;      // Zero-width - forces alignment
    unsigned int b : 4;
};

//===----------------------------------------------------------------------===//
// Edge Case: Named zero-width (error in most compilers)
// struct NamedZeroWidth {
//     unsigned int x : 0;  // Would be an error
// };
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Edge Case: Single-bit signed (should warn for 6.2)
//===----------------------------------------------------------------------===//

struct SingleBitSigned {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid signed type for bitfields of single bit
    int flag : 1;  // Signed single-bit can only be 0 or -1 (int type is OK per MISRA 6.1)
};

struct SingleBitUnsigned {
    unsigned int flag : 1;  // OK - can be 0 or 1
};

struct SingleBitBool {
    _Bool flag : 1;  // OK - boolean single bit
};

//===----------------------------------------------------------------------===//
// Edge Case: Maximum width bitfield
//===----------------------------------------------------------------------===//

struct MaxWidth {
    unsigned int full : 32;  // Maximum width for unsigned int
    // unsigned int overflow : 33;  // Would be compile error
};

//===----------------------------------------------------------------------===//
// Edge Case: Bitfield in union (should warn for 19.2)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: avoid bitfield in union
union BitfieldUnion {
    unsigned int bits : 8;
    unsigned char byte;
};

//===----------------------------------------------------------------------===//
// Edge Case: Anonymous bitfield (padding)
//===----------------------------------------------------------------------===//

struct AnonymousBitfield {
    unsigned int a : 4;
    unsigned int : 4;   // Anonymous padding - should not warn
    unsigned int b : 4;
};

//===----------------------------------------------------------------------===//
// Edge Case: Typedef'd type as bitfield
//===----------------------------------------------------------------------===//

typedef unsigned int uint32;
typedef enum { A, B, C } MyEnum;

struct TypedefBitfield {
    uint32 value : 8;           // Typedef of unsigned int - OK

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: wrong type in bitfield
    MyEnum state : 2;           // Typedef of enum - should warn
};

//===----------------------------------------------------------------------===//
// Edge Case: Multiple bitfields with various issues
//===----------------------------------------------------------------------===//

struct MultipleBitfields {
    unsigned int ok1 : 4;       // OK
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid signed type for bitfields of single bit
    int bad1 : 1;               // Signed single-bit (int type is OK per MISRA 6.1)
    unsigned int ok2 : 8;       // OK
    _Bool ok3 : 1;              // OK
};

//===----------------------------------------------------------------------===//
// Compliant Cases
//===----------------------------------------------------------------------===//

struct CompliantBitfields {
    unsigned int flags : 4;
    signed int value : 8;       // Signed int - OK per MISRA 6.1
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: wrong type in bitfield
    unsigned char small : 4;    // unsigned char not allowed - only int types and _Bool
    _Bool boolean : 1;
    unsigned int : 0;           // Zero-width alignment
};

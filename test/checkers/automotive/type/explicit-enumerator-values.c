// Test file for: automotive-explicit-enumerator-values
// Related MISRA C:2025 Rule: 8.12
//
// This file tests the detection of enumerators without explicit values

// RUN: %check_clang_tidy %s automotive-explicit-enumerator-values %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Enum with no explicit values (all implicit)
enum Status {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator OK has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    OK,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator ERROR has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    ERROR,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator PENDING has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    PENDING
};

// Enum with mixed explicit and implicit values
enum Color {
    RED = 0,     // Explicit - OK
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator GREEN has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    GREEN,       // Implicit (will be 1)
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator BLUE has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    BLUE         // Implicit (will be 2)
};

// Enum with first value explicit, rest implicit
enum Priority {
    LOW = 1,     // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator MEDIUM has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    MEDIUM,      // Implicit (will be 2)
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator HIGH has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    HIGH         // Implicit (will be 3)
};

// Enum with some explicit values in middle
enum Direction {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator NORTH has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    NORTH,           // Implicit (0)
    EAST = 90,       // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator SOUTH has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    SOUTH,           // Implicit (91)
    WEST = 270       // Explicit
};

// Enum with large gaps
enum Flags {
    FLAG_A = 0x01,   // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator FLAG_B has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    FLAG_B,          // Implicit (0x02)
    FLAG_C = 0x04,   // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator FLAG_D has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    FLAG_D           // Implicit (0x05)
};

// Single enumerator without value
enum Single {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator ONLY has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    ONLY
};

// Typedef enum with implicit values
typedef enum {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator TYPE_A has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    TYPE_A,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator TYPE_B has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    TYPE_B,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator TYPE_C has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
    TYPE_C
} TypeEnum;

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Enum with all explicit values
enum ExplicitStatus {
    EXPLICIT_OK = 0,
    EXPLICIT_ERROR = 1,
    EXPLICIT_PENDING = 2
};

// Enum with explicit hex values
enum HexValues {
    HEX_A = 0x00,
    HEX_B = 0x01,
    HEX_C = 0x02,
    HEX_D = 0xFF
};

// Enum with explicit bit flags
enum BitFlags {
    BIT_0 = (1 << 0),
    BIT_1 = (1 << 1),
    BIT_2 = (1 << 2),
    BIT_3 = (1 << 3)
};

// Enum with negative values
enum Signed {
    NEG = -1,
    ZERO = 0,
    POS = 1
};

// Enum with explicit sequential values
enum Sequential {
    SEQ_0 = 0,
    SEQ_1 = 1,
    SEQ_2 = 2,
    SEQ_3 = 3,
    SEQ_4 = 4
};

// Single enumerator with explicit value
enum SingleExplicit {
    SINGLE_VALUE = 42
};

// Typedef enum with all explicit values
typedef enum {
    TYPEDEF_A = 10,
    TYPEDEF_B = 20,
    TYPEDEF_C = 30
} TypedefExplicitEnum;

// Enum with expression values
enum Expressions {
    EXPR_A = 5,
    EXPR_B = 10,
    EXPR_C = EXPR_A + EXPR_B,  // 15
    EXPR_D = EXPR_C * 2        // 30
};

// Enum with calculation expressions
enum Calculated {
    CALC_BASE = 100,
    CALC_OFFSET = 50,
    CALC_TOTAL = CALC_BASE + CALC_OFFSET
};

// Enum with all same value (unusual but explicit)
enum AllSame {
    SAME_A = 0,
    SAME_B = 0,
    SAME_C = 0
};

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

// Empty enum (no enumerators) - should not crash
enum Empty {
};

// Enum with trailing comma and explicit values
enum TrailingComma {
    TC_A = 1,
    TC_B = 2,
    TC_C = 3,
};

// Enum with explicit zero value
enum ExplicitZero {
    EZ_FIRST = 0,
    EZ_SECOND = 1
};

// Enum used in typedef with explicit values
typedef enum EnumTag {
    ET_VALUE_1 = 100,
    ET_VALUE_2 = 200
} EnumTypedef;

// Enum with large values
enum LargeValues {
    LARGE_A = 1000000,
    LARGE_B = 2000000,
    LARGE_C = 3000000
};

// Enum with char literal values
enum CharValues {
    CHAR_A = 'A',
    CHAR_B = 'B',
    CHAR_C = 'C'
};

// Enum with octal values (explicit)
enum OctalValues {
    OCTAL_A = 010,  // 8 in decimal
    OCTAL_B = 020,  // 16 in decimal
    OCTAL_C = 030   // 24 in decimal
};

// Enum forward declaration (should be skipped)
enum ForwardDecl;

// Definition of forward declared enum
enum ForwardDecl {
    FD_A = 1,
    FD_B = 2
};

// Enum with sizeof expressions
enum SizeofValues {
    SIZE_CHAR = sizeof(char),
    SIZE_INT = sizeof(int),
    SIZE_LONG = sizeof(long)
};

// Enum with mixed case but all explicit
enum MixedCase {
    mixedLower = 0,
    MIXED_UPPER = 1,
    MixedCamel = 2
};

// Enum in different contexts
void test_function(void) {
    // Enum declared in function scope
    enum LocalEnum {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: enumerator LOCAL_A has implicit value; each enumerator should have an explicit value [automotive-explicit-enumerator-values]
        LOCAL_A,
        LOCAL_B = 2
    };

    // Using compliant enum
    enum LocalCompliant {
        LC_A = 0,
        LC_B = 1
    };
}

// Enum with maximum value
enum MaxValue {
    MAX_VAL = 0x7FFFFFFF  // Maximum positive int
};

// Enum used as bit mask (all explicit)
enum BitMask {
    MASK_NONE = 0x00,
    MASK_READ = 0x01,
    MASK_WRITE = 0x02,
    MASK_EXECUTE = 0x04,
    MASK_ALL = MASK_READ | MASK_WRITE | MASK_EXECUTE
};

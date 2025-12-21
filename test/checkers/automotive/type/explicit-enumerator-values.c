// RUN: %check_clang_tidy %s automotive-c23-req-9.2 %t

// Test: Enumerators without explicit values (MISRA Rule 9.2)

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Enum with no explicit values (all implicit)
enum Status {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'OK' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    OK,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'ERROR' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    ERROR,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'PENDING' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    PENDING
};

// Enum with mixed explicit and implicit values
enum Color {
    RED = 0,     // Explicit - OK
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'GREEN' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    GREEN,       // Implicit (will be 1)
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'BLUE' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    BLUE         // Implicit (will be 2)
};

// Enum with first value explicit, rest implicit
enum Priority {
    LOW = 1,     // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'MEDIUM' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    MEDIUM,      // Implicit (will be 2)
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'HIGH' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    HIGH         // Implicit (will be 3)
};

// Enum with some explicit values in middle
enum Direction {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'NORTH' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    NORTH,           // Implicit (0)
    EAST = 90,       // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'SOUTH' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    SOUTH,           // Implicit (91)
    WEST = 270       // Explicit
};

// Enum with large gaps
enum Flags {
    FLAG_A = 0x01,   // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'FLAG_B' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    FLAG_B,          // Implicit (0x02)
    FLAG_C = 0x04,   // Explicit
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'FLAG_D' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    FLAG_D           // Implicit (0x05)
};

// Single enumerator without value
enum Single {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'ONLY' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    ONLY
};

// Typedef enum with implicit values
typedef enum {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'TYPE_A' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    TYPE_A,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'TYPE_B' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
    TYPE_B,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: enumerator 'TYPE_C' has implicit value; each enumerator should have an explicit value [automotive-c23-req-9.2]
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

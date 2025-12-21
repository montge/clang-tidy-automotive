// Test file for: automotive-unique-enum-value
// Related MISRA C:2025 Rule: 9.5
//
// This file tests the detection of duplicate enum values

// RUN: %check_clang_tidy %s automotive-unique-enum-value %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

enum BadEnum1 {
    VALUE_A = 0,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: duplicate enum value '0' from 'VALUE_A'
    VALUE_B = 0,  // Duplicate value
    // CHECK-MESSAGES: :[[@LINE-3]]:5: note: declaration of 'VALUE_A'
    VALUE_C = 1
};

enum BadEnum2 {
    FIRST = 1,
    SECOND = 2,
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: duplicate enum value '2' from 'SECOND'
    THIRD = 2,    // Duplicate of SECOND
    // CHECK-MESSAGES: :[[@LINE-3]]:5: note: declaration of 'SECOND'
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: duplicate enum value '1' from 'FIRST'
    FOURTH = 1    // Duplicate of FIRST
    // CHECK-MESSAGES: :[[@LINE-7]]:5: note: declaration of 'FIRST'
};

// Implicit duplicates
enum BadEnum3 {
    A = 5,
    B,        // B = 6
    C,        // C = 7
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: duplicate enum value '6' from 'B'
    D = 6     // Duplicate of B
    // CHECK-MESSAGES: :[[@LINE-4]]:5: note: declaration of 'B'
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// All unique values
enum GoodEnum1 {
    OK_A = 0,
    OK_B = 1,
    OK_C = 2,
    OK_D = 3
};

// Implicit unique values
enum GoodEnum2 {
    G1,  // 0
    G2,  // 1
    G3,  // 2
    G4   // 3
};

// Mixed explicit and implicit
enum GoodEnum3 {
    M1 = 10,
    M2,       // 11
    M3 = 20,
    M4        // 21
};

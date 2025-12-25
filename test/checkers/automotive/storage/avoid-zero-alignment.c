// RUN: %check_clang_tidy %s automotive-c25-adv-8.16 %t

// Test: Zero alignment specifier (MISRA C:2025 Rule 8.16)
// Check ID: automotive-c25-adv-8.16

// ============= Compliant: Valid alignment values =============

_Alignas(4) int aligned4;
_Alignas(8) double aligned8;
_Alignas(16) char buffer[16];

struct AlignedStruct {
    _Alignas(8) int member;
};

// ============= Violations: Zero alignment =============

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: alignment specifier with value zero
_Alignas(0) int zero_aligned;

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: alignment specifier with value zero
    _Alignas(0) char zero_buffer[4];

struct BadStruct {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: alignment specifier with value zero
    _Alignas(0) int bad_member;
};

// ============= Compliant: Expression alignment =============

#define ALIGNMENT 16
_Alignas(ALIGNMENT) int macro_aligned;

// Computed alignment (not zero)
_Alignas(sizeof(double)) int size_aligned;

// ============= Edge cases =============

// Multiple declarations
_Alignas(4) int multi1;
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: alignment specifier with value zero
_Alignas(0) int multi2;
_Alignas(8) int multi3;

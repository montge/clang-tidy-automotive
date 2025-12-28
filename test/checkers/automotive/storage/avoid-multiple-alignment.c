// RUN: %check_clang_tidy %s automotive-c25-adv-8.17 %t

// Test: Multiple alignment specifiers (MISRA C:2025 Rule 8.17)
// Check ID: automotive-c25-adv-8.17

// ============= Compliant: Single alignment specifier =============

_Alignas(4) int single_aligned;
_Alignas(8) double single_double;
_Alignas(16) char single_buffer[16];

struct SingleAligned {
    _Alignas(8) int member;
};

// ============= Violations: Multiple alignment specifiers =============

// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: multiple alignment specifiers on single declaration; use only one alignment specifier per declaration [automotive-c25-adv-8.17]
_Alignas(4) _Alignas(8) int multi_aligned;

// CHECK-MESSAGES: :[[@LINE+1]]:14: warning: multiple alignment specifiers on single declaration; use only one alignment specifier per declaration [automotive-c25-adv-8.17]
_Alignas(16) _Alignas(32) double multi_double;

struct MultiAligned {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: multiple alignment specifiers on single declaration; use only one alignment specifier per declaration [automotive-c25-adv-8.17]
    _Alignas(4) _Alignas(16) int multi_member;
};

// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: multiple alignment specifiers on single declaration; use only one alignment specifier per declaration [automotive-c25-adv-8.17]
_Alignas(8) _Alignas(8) int same_alignment;  // Even same value is problematic

// ============= Compliant: Alignment on type vs variable =============

// This is a single specifier on the variable
typedef int AlignedInt;
_Alignas(16) AlignedInt typedef_aligned;  // OK - single specifier

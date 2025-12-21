// RUN: %check_clang_tidy %s automotive-avoid-undef %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-undef %t -- -- -std=c11

// MISRA C:2025 Rule 20.5 (Advisory)
// The #undef directive should not be used.

#define PASS 0
#undef PASS
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: avoid #undef [automotive-avoid-undef]

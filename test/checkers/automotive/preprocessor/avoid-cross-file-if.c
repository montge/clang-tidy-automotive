// RUN: %check_clang_tidy %s automotive-c23-req-20.14 %t
// Test for automotive-c23-req-20.14
// Related MISRA C:2025 Rule: 20.14

// This test verifies that #if/#endif directives are in the same file.
// Note: Testing cross-file violations requires header files, which is
// complex for lit tests. This test shows compliant patterns.

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// #if and #endif in same file - compliant
#if 1
int compliant1 = 1;
#endif

// #ifdef and #endif in same file - compliant
#define FEATURE_A
#ifdef FEATURE_A
int compliant2 = 2;
#endif

// #ifndef and #endif in same file - compliant
#ifndef UNDEFINED_MACRO
int compliant3 = 3;
#endif

// #if with #else and #endif in same file - compliant
#if 0
int never_compiled = 0;
#else
int compliant4 = 4;
#endif

// Nested #if/#endif in same file - compliant
#if 1
  #if 1
  int nested = 1;
  #endif
#endif

// #if with #elif in same file - compliant
#define MODE 2
#if MODE == 1
int mode1 = 1;
#elif MODE == 2
int mode2 = 2;
#else
int default_mode = 0;
#endif

// Note: Cross-file violations (e.g., #if in header, #endif in .c file)
// would trigger warnings but require multi-file test setup.

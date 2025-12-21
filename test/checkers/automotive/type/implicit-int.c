// RUN: %check_clang_tidy %s automotive-implicit-int %t -- -- -Wno-implicit-int -Wno-implicit-function-declaration

// Test file for: automotive-implicit-int
// Related MISRA C:2025 Rule: 8.1
// This file tests the detection of implicit int declarations

// Implicit int with unsigned modifier
// CHECK-MESSAGES: :[[@LINE+1]]:10: warning: implicit int type in declaration [automotive-implicit-int]
unsigned z = 20;

// Compliant Cases - Explicit types
int explicit_int = 5;
unsigned int explicit_uint = 10;
const int explicit_const = 15;
static int explicit_static = 20;

// Explicit function return type
int func2(void) {
    return 0;
}

// Explicit void return
void func3(void) {
    return;
}

// RUN: %check_clang_tidy %s automotive-cpp23-req-7.2 %t
// Test for automotive-cpp23-req-7.2
// Related MISRA C++:2023 Rule: 7.2 - Unsigned integer operations shall not wrap around

//===----------------------------------------------------------------------===//
// Violation Cases - Subtraction
//===----------------------------------------------------------------------===//

void test_subtraction_basic() {
    unsigned int a = 10;
    unsigned int b = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: unsigned integer subtraction '-' may wrap around
    unsigned int c = a - b;
    (void)c;
}

void test_subtraction_unsigned_long() {
    unsigned long a = 100UL;
    unsigned long b = 200UL;
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: unsigned integer subtraction '-' may wrap around
    unsigned long c = a - b;
    (void)c;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Addition
//===----------------------------------------------------------------------===//

void test_addition_variables() {
    unsigned int x = 1000000;
    unsigned int y = 2000000;
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: unsigned integer addition '+' may wrap around
    unsigned int z = x + y;
    (void)z;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Multiplication
//===----------------------------------------------------------------------===//

void test_multiplication_variables() {
    unsigned int x = 1000000;
    unsigned int y = 5000;
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: unsigned integer multiplication '*' may wrap around
    unsigned int z = x * y;
    (void)z;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_signed_subtraction() {
    int a = 10;
    int b = 20;
    int c = a - b;  // OK: signed arithmetic
    (void)c;
}

void test_increment() {
    unsigned int a = 10;
    a++;  // OK: increment is typically safe for single operations
    (void)a;
}

void test_decrement() {
    unsigned int a = 10;
    a--;  // OK: decrement is typically safe for single operations
    (void)a;
}

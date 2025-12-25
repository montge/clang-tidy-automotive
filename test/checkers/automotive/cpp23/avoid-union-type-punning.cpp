// RUN: %check_clang_tidy %s automotive-cpp23-req-19.1 %t -- -- -std=c++11
// Test for automotive-cpp23-req-19.1: Type-punning through union members
// Related MISRA C++:2023 Rule: 19.1

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

union Data {
  int i;
  float f;
  char c;
};

union TypePunning {
  int i;
  unsigned int u;
  float f;
  double d;
};

void test_type_punning() {
  Data d1;
  d1.i = 42;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: union member 'f' accessed after writing to member 'i' (type-punning from int to float) [automotive-cpp23-req-19.1]
  float x = d1.f;  // Warning: type-punning int to float

  Data d2;
  d2.f = 3.14f;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: union member 'i' accessed after writing to member 'f' (type-punning from float to int) [automotive-cpp23-req-19.1]
  int y = d2.i;  // Warning: type-punning float to int

  TypePunning tp;
  tp.i = 100;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: union member 'f' accessed after writing to member 'i' (type-punning from int to float) [automotive-cpp23-req-19.1]
  float z = tp.f;  // Warning: type-punning int to float

  TypePunning tp2;
  tp2.f = 1.5f;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: union member 'd' accessed after writing to member 'f' (type-punning from float to double) [automotive-cpp23-req-19.1]
  double w = tp2.d;  // Warning: type-punning float to double
}

void test_multiple_punning() {
  Data d;
  d.i = 10;
  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: union member 'f' accessed after writing to member 'i' (type-punning from int to float) [automotive-cpp23-req-19.1]
  if (d.f > 0.0f) {  // Warning: type-punning in conditional
    // Do something
  }
}

void test_punning_in_expression() {
  TypePunning tp;
  tp.i = 42;
  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: union member 'f' accessed after writing to member 'i' (type-punning from int to float) [automotive-cpp23-req-19.1]
  float result = tp.f + 1.0f;  // Warning: type-punning in expression
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_same_member_access() {
  Data d;
  d.i = 42;
  int x = d.i;  // OK: reading from same member that was written
}

void test_char_access() {
  Data d;
  d.i = 65;
  char ch = d.c;  // OK: char can alias other types (common initial sequence)
}

void test_signed_unsigned_compatibility() {
  TypePunning tp;
  tp.i = -1;
  unsigned int u = tp.u;  // OK: signed/unsigned variants are compatible
}

void test_sequential_writes() {
  Data d;
  d.i = 10;
  d.f = 3.14f;  // OK: just writing, not reading previous value
  float x = d.f;  // OK: reading from same member that was last written
}

void test_no_previous_write() {
  Data d;
  // No warning - union not initialized/written to yet
  // (implementation may not track this case)
}

void test_proper_conversion() {
  // Compliant alternative: use proper conversion
  int i = 42;
  float f = static_cast<float>(i);  // OK: explicit conversion

  // Or in C++20, use std::bit_cast
  // auto f = std::bit_cast<float>(i);
}

void test_memcpy_alternative() {
  // Compliant alternative: use memcpy for bit conversion
  int i = 42;
  float f;
  __builtin_memcpy(&f, &i, sizeof(float));  // OK: explicit bit copy
}

void test_rewrite_same_type() {
  Data d;
  d.i = 10;
  d.i = 20;  // OK: overwriting same member
  int x = d.i;  // OK: reading from same member
}

// Test with different union
union AnotherUnion {
  long l;
  double d;
};

void test_different_unions() {
  Data d1;
  d1.i = 1;

  AnotherUnion au;
  au.l = 100L;

  float x = d1.i;  // OK: reading same type as written
  long y = au.l;   // OK: reading same type as written
}

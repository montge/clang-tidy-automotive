// RUN: %check_clang_tidy %s automotive-unique-enum-value %t -- -- -std=c11
// Test for automotive-unique-enum-value: enum constants shall have unique values

enum DuplicateValues {
  VALUE_A = 1,
  VALUE_B = 2,
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: duplicate enum value '1' from 'VALUE_A' [automotive-unique-enum-value]
  VALUE_C = 1,  // Duplicate of VALUE_A
  VALUE_D = 3
};

enum AnotherDuplicate {
  FIRST = 0,
  SECOND = 1,
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: duplicate enum value '0' from 'FIRST' [automotive-unique-enum-value]
  THIRD = 0,  // Duplicate of FIRST
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: duplicate enum value '1' from 'SECOND' [automotive-unique-enum-value]
  FOURTH = 1  // Duplicate of SECOND
};

// OK - all unique values
enum UniqueValues {
  UNIQUE_A = 0,
  UNIQUE_B = 1,
  UNIQUE_C = 2,
  UNIQUE_D = 3
};

// OK - implicit unique values
enum ImplicitUnique {
  IMPLICIT_A,  // 0
  IMPLICIT_B,  // 1
  IMPLICIT_C,  // 2
  IMPLICIT_D   // 3
};

// OK - mixed implicit and explicit, all unique
enum MixedUnique {
  MIXED_A = 10,
  MIXED_B,      // 11
  MIXED_C = 20,
  MIXED_D       // 21
};

// OK - negative values, all unique
enum NegativeUnique {
  NEG_A = -2,
  NEG_B = -1,
  NEG_C = 0,
  NEG_D = 1
};

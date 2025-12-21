// RUN: %check_clang_tidy %s automotive-cpp23-req-15.1.2 %t
// Test for automotive-cpp23-req-15.1.2: Conversion operators shall be explicit

class BadConverter {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: conversion operator to 'int' should be explicit to avoid unintended implicit conversions
  operator int() const { return value_; }

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: conversion operator to 'bool' should be explicit to avoid unintended implicit conversions
  operator bool() const { return value_ != 0; }

private:
  int value_ = 0;
};

class GoodConverter {
public:
  // Explicit conversion operators are compliant
  explicit operator int() const { return value_; }

  explicit operator bool() const { return value_ != 0; }

private:
  int value_ = 0;
};

void test_conversions() {
  BadConverter bad;
  int x = bad;       // Implicit conversion - allowed by bad design
  bool b = bad;      // Implicit conversion - allowed by bad design
  (void)x;
  (void)b;

  GoodConverter good;
  int y = static_cast<int>(good);    // Explicit conversion required
  bool c = static_cast<bool>(good);  // Explicit conversion required
  (void)y;
  (void)c;
}

// RUN: %check_clang_tidy %s automotive-cpp23-adv-10.4.1 %t
// Test for automotive-cpp23-adv-10.4.1: A conversion function shall be explicit

class ImplicitConverter {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: conversion function to 'int' shall be marked explicit to prevent implicit conversions [automotive-cpp23-adv-10.4.1]
  operator int() const { return value_; }

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: conversion function to 'bool' shall be marked explicit to prevent implicit conversions [automotive-cpp23-adv-10.4.1]
  operator bool() const { return value_ != 0; }

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: conversion function to 'double' shall be marked explicit to prevent implicit conversions [automotive-cpp23-adv-10.4.1]
  operator double() const { return static_cast<double>(value_); }

private:
  int value_ = 0;
};

class ExplicitConverter {
public:
  // Compliant: explicit conversion operators
  explicit operator int() const { return value_; }

  explicit operator bool() const { return value_ != 0; }

  explicit operator double() const { return static_cast<double>(value_); }

private:
  int value_ = 0;
};

// Test with templates
template<typename T>
class TemplateConverter {
public:
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: conversion function to 'T' shall be marked explicit to prevent implicit conversions [automotive-cpp23-adv-10.4.1]
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: conversion function to 'int' shall be marked explicit to prevent implicit conversions [automotive-cpp23-adv-10.4.1]
  operator T() const { return val_; }

private:
  T val_;
};

// Compliant template version
template<typename T>
class ExplicitTemplateConverter {
public:
  explicit operator T() const { return val_; }

private:
  T val_;
};

void test_usage() {
  ImplicitConverter implicit;

  // These implicit conversions are allowed (but shouldn't be)
  int i = implicit;
  bool b = implicit;
  double d = implicit;
  (void)i;
  (void)b;
  (void)d;

  ExplicitConverter explicit_conv;

  // These require explicit casts (good)
  int j = static_cast<int>(explicit_conv);
  bool c = static_cast<bool>(explicit_conv);
  double e = static_cast<double>(explicit_conv);
  (void)j;
  (void)c;
  (void)e;

  TemplateConverter<int> template_implicit;
  int k = template_implicit;  // Implicit conversion
  (void)k;

  ExplicitTemplateConverter<int> template_explicit;
  int l = static_cast<int>(template_explicit);  // Explicit conversion required
  (void)l;
}

// Test deleted conversion operators (should not warn)
class DeletedConverter {
public:
  operator int() const = delete;
  operator bool() const = delete;
};

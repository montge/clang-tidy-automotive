// RUN: %check_clang_tidy %s automotive-cpp23-req-8.3.1 %t
// Test for automotive-cpp23-req-8.3.1: Implicit conversions shall be explicit

void takeDouble(double d) { (void)d; }
void takeBool(bool b) { (void)b; }
void takeUnsigned(unsigned u) { (void)u; }

void test_implicit_conversions() {
  int x = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: implicit conversion from 'int' to 'double'; consider using an explicit cast
  takeDouble(x);

  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: implicit conversion from 'int' to 'bool'; consider using an explicit cast
  takeBool(x);

  signed int s = -1;
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: implicit conversion from 'int' to 'unsigned int'; consider using an explicit cast
  takeUnsigned(s);
}

void test_explicit_conversions() {
  int x = 42;

  // OK: explicit conversion
  takeDouble(static_cast<double>(x));

  // OK: explicit conversion
  takeBool(static_cast<bool>(x));

  signed int s = -1;
  // OK: explicit conversion
  takeUnsigned(static_cast<unsigned>(s));
}

double return_implicit() {
  int x = 42;
  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: implicit conversion from 'int' to 'double'; consider using an explicit cast
  return x;
}

double return_explicit() {
  int x = 42;
  return static_cast<double>(x);  // OK
}

// RUN: %check_clang_tidy %s automotive-c25-req-23.4 %t -- -- -std=c11

// Test for appropriate types in _Generic associations
// Note: Many invalid cases are caught by the compiler itself (errors)
// This check provides MISRA-specific messaging for additional cases

struct Complete { int x; };

int complete_func(int x);

// Compliant test cases - all should pass without warnings from this check

void test_compliant_basic_types(void) {
  int x = 1;
  float f = 1.0f;

  // Compliant: all basic object types
  int r1 = _Generic(x,
    int: 1,
    float: 2,
    double: 3,
    default: 0
  );

  (void)r1;
}

void test_compliant_pointer_types(void) {
  int x = 1;

  // Compliant: pointer types
  int r1 = _Generic(&x,
    int*: 1,
    float*: 2,
    void*: 3,
    default: 0
  );

  (void)r1;
}

void test_compliant_struct_types(void) {
  struct Complete s = {0};

  // Compliant: struct types
  int r1 = _Generic(s,
    struct Complete: 1,
    default: 0
  );

  (void)r1;
}

void test_compliant_function_pointer(void) {
  // Compliant: pointer to function (not function type)
  int (*fp)(int) = complete_func;
  int r1 = _Generic(fp,
    int(*)(int): 1,
    default: 0
  );

  (void)r1;
}

void test_default_only(void) {
  int x = 1;

  // Compliant: only default association
  int r1 = _Generic(x,
    default: 0
  );

  (void)r1;
}

void test_multiple_types(void) {
  long x = 1L;

  // Compliant: various compatible types
  int r1 = _Generic(x,
    char: 1,
    short: 2,
    int: 3,
    long: 4,
    long long: 5,
    default: 0
  );

  (void)r1;
}

void test_unsigned_types(void) {
  unsigned int x = 1U;

  // Compliant: unsigned types
  int r1 = _Generic(x,
    unsigned char: 1,
    unsigned short: 2,
    unsigned int: 3,
    unsigned long: 4,
    default: 0
  );

  (void)r1;
}

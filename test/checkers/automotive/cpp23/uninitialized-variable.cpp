// RUN: %check_clang_tidy %s automotive-cpp23-adv-11.6.1 %t

// MISRA C++:2023 Rule 11.6.1
// All variables shall be initialized before first use

// Test case 1: Local variables
void testLocalVariables() {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: variable 'x' is not initialized
  int x;

  int y = 0; // OK - initialized

  int z{}; // OK - initialized with brace initializer

  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: variable 'd' is not initialized
  double d;

  double e = 3.14; // OK - initialized

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: variable 'c' is not initialized
  char c;

  char initialized_c = 'a'; // OK - initialized

  (void)x; (void)y; (void)z; (void)d; (void)e; (void)c; (void)initialized_c;
}

// Test case 2: Pointers
void testPointers() {
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: variable 'ptr' is not initialized
  int *ptr;

  int *initialized_ptr = nullptr; // OK - initialized

  (void)ptr; (void)initialized_ptr;
}

// Test case 3: Static variables
void testStaticVariables() {
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: static variable 's' is not explicitly initialized
  static int s;

  static int initialized_s = 0; // OK - explicitly initialized

  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: static variable 'd' is not explicitly initialized
  static double d;

  (void)s; (void)initialized_s; (void)d;
}

// Test case 4: Reference types (should not warn - compiler enforces)
void testReferences(int &ref) {
  int value = 10;
  int &local_ref = value; // OK - references must be initialized
  (void)ref; (void)local_ref;
}

// Test case 5: constexpr variables (should not warn - compiler enforces)
void testConstexpr() {
  constexpr int ce = 5; // OK - constexpr must be initialized
  (void)ce;
}

// Test case 6: Multiple declarations
void testMultipleDeclarations() {
  // CHECK-MESSAGES: :[[@LINE+2]]:7: warning: variable 'a' is not initialized
  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: variable 'b' is not initialized
  int a, b;

  int c = 1, d = 2; // OK - both initialized

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: variable 'f' is not initialized
  int e = 3, f;

  (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}

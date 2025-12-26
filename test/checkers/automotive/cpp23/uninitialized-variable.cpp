// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-11.6.1 %t

// MISRA C++:2023 Rule 11.6.1
// All variables shall be initialized before first use

#include <string>
#include <vector>

// Test case 1: Local variables
void testLocalVariables() {
  int x;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: variable 'x' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

  int y = 0; // OK - initialized

  int z{}; // OK - initialized with brace initializer

  double d;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: variable 'd' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

  double e = 3.14; // OK - initialized

  char c;
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: variable 'c' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

  char initialized_c = 'a'; // OK - initialized
}

// Test case 2: Types with default constructors
void testTypesWithConstructors() {
  std::string s; // OK - std::string has a default constructor that initializes

  std::vector<int> vec; // OK - vector has a default constructor

  int *ptr;
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: variable 'ptr' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

  int *initialized_ptr = nullptr; // OK - initialized
}

// Test case 3: Static variables
void testStaticVariables() {
  static int s;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: static variable 's' is not explicitly initialized; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

  static int initialized_s = 0; // OK - explicitly initialized

  static double d;
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: static variable 'd' is not explicitly initialized; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]
}

// Test case 4: Global static variables
static int global_static;
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: static variable 'global_static' is not explicitly initialized; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

static int initialized_global_static = 42; // OK - explicitly initialized

// Test case 5: Reference types (should not warn - compiler enforces)
void testReferences(int &ref) {
  int value = 10;
  int &local_ref = value; // OK - references must be initialized
}

// Test case 6: constexpr variables (should not warn - compiler enforces)
void testConstexpr() {
  constexpr int ce = 5; // OK - constexpr must be initialized
}

// Test case 7: Parameters (should not warn)
void testParameters(int param) {
  // param is initialized by caller - no warning expected
}

// Test case 8: Class member variables
class TestClass {
  int uninitialized_member;
  int initialized_member = 0; // OK - has in-class initializer
  std::string str_member; // OK - has default constructor

public:
  // Constructor with initializer list
  TestClass() : initialized_member(5) {
    // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: member variable 'uninitialized_member' is not initialized in constructor; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]
    // CHECK-MESSAGES: :[[@LINE-9]]:7: note: member declared here
  }

  // Constructor that initializes all members
  TestClass(int value) : uninitialized_member(value), initialized_member(10) {
    // OK - all non-default-constructed members initialized
  }
};

class TestClass2 {
  int member1;
  int member2;
  double member3 = 0.0; // OK - has in-class initializer

public:
  TestClass2() : member1(0) {
    // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: member variable 'member2' is not initialized in constructor; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]
    // CHECK-MESSAGES: :[[@LINE-6]]:7: note: member declared here
  }
};

class CompliantClass {
  int member1 = 0;
  int member2;

public:
  CompliantClass() : member2(42) {
    // OK - all members initialized (member1 has in-class initializer)
  }
};

// Test case 9: Struct with trivial default constructor
struct TrivialStruct {
  int a;
  double b;
};

void testTrivialStruct() {
  TrivialStruct ts;
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: variable 'ts' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

  TrivialStruct ts2{}; // OK - aggregate initialization

  TrivialStruct ts3 = {1, 2.0}; // OK - aggregate initialization with values
}

// Test case 10: Struct with non-trivial default constructor
struct NonTrivialStruct {
  std::string s;
  int value;

  NonTrivialStruct() : value(0) {} // OK - has non-trivial constructor
};

void testNonTrivialStruct() {
  NonTrivialStruct nts; // OK - has non-trivial default constructor
}

// Test case 11: Arrays (should not warn - different check)
void testArrays() {
  int arr[10]; // Arrays are handled by a different check
}

// Test case 12: Extern declarations (should not warn)
extern int external_var; // OK - extern declaration

// Test case 13: Multiple declarations
void testMultipleDeclarations() {
  int a, b;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: variable 'a' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]
  // CHECK-MESSAGES: :[[@LINE-2]]:10: warning: variable 'b' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]

  int c = 1, d = 2; // OK - both initialized

  int e = 3, f;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: variable 'f' is not initialized at declaration; all variables shall be initialized before first use [automotive-cpp23-req-11.6.1]
}

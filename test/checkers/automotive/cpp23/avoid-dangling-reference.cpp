// RUN: %check_clang_tidy -std=c++11 %s automotive-cpp23-req-7.5.1 %t

// MISRA C++:2023 Rule 7.5.1: A function shall not return a reference or a
// pointer to an automatic variable.

// Test case 1: Return address of local variable
int* bad_return_address() {
  int local = 42;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function returns address of local variable 'local' with automatic storage duration
  return &local;
}

// Test case 2: Return reference to local variable
int& bad_return_reference() {
  int local = 42;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function returns reference to local variable 'local' with automatic storage duration
  return local;
}

// Test case 3: Return address from nested scope
int* bad_nested_scope() {
  {
    int local = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function returns address of local variable 'local' with automatic storage duration
    return &local;
  }
}

// Test case 4: Return reference to const local (still bad)
const int& bad_return_const_ref() {
  int local = 42;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function returns reference to local variable 'local' with automatic storage duration
  return local;
}

// Test case 5: Struct local variable
struct Point {
  int x, y;
};

Point* bad_return_struct() {
  Point p{1, 2};
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function returns address of local variable 'p' with automatic storage duration
  return &p;
}

// Compliant cases (no warnings expected)

// Return by value (OK)
int good_return_by_value() {
  int local = 42;
  return local;
}

// Return pointer to static variable (OK)
int* good_return_static() {
  static int persistent = 42;
  return &persistent;
}

// Return reference to static variable (OK)
int& good_return_static_ref() {
  static int persistent = 42;
  return persistent;
}

// Global variable (OK)
int global_var = 100;

int* good_return_global() {
  return &global_var;
}

int& good_return_global_ref() {
  return global_var;
}

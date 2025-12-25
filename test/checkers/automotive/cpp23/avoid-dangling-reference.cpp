// RUN: %check_clang_tidy %s automotive-cpp23-req-7.5.1 %t

// MISRA C++:2023 Rule 7.5.1: A function shall not return a reference or a
// pointer to an automatic variable.

// Test case 1: Return address of local variable
int* bad_return_address() {
  int local = 42;
  return &local; // CHECK-MESSAGES: :[[@LINE]]:3: warning: function returns address of local variable 'local' with automatic storage duration [automotive-cpp23-req-7.5.1]
  // CHECK-MESSAGES: :[[@LINE-2]]:7: note: variable declared here
}

// Test case 2: Return reference to local variable
int& bad_return_reference() {
  int local = 42;
  return local; // CHECK-MESSAGES: :[[@LINE]]:3: warning: function returns reference to local variable 'local' with automatic storage duration [automotive-cpp23-req-7.5.1]
  // CHECK-MESSAGES: :[[@LINE-2]]:7: note: variable declared here
}

// Test case 3: Return pointer to local variable via intermediate pointer
int* bad_return_pointer() {
  int local = 42;
  int* ptr = &local;
  return ptr; // CHECK-MESSAGES: :[[@LINE]]:3: warning: function returns pointer variable 'ptr' that refers to local variable 'local' with automatic storage duration [automotive-cpp23-req-7.5.1]
  // CHECK-MESSAGES: :[[@LINE-3]]:7: note: referenced variable declared here
  // CHECK-MESSAGES: :[[@LINE-3]]:8: note: pointer variable declared here
}

// Test case 4: Return by value (OK)
int good_return_by_value() {
  int local = 42;
  return local; // OK: return by value
}

// Test case 5: Return pointer to static variable (OK)
int* good_return_static() {
  static int persistent = 42;
  return &persistent; // OK: static storage duration
}

// Test case 6: Return reference to static variable (OK)
int& good_return_static_ref() {
  static int persistent = 42;
  return persistent; // OK: static storage duration
}

// Test case 7: Return pointer to heap-allocated memory (OK)
int* good_return_heap() {
  int* ptr = new int(42);
  return ptr; // OK: heap allocation
}

// Test case 8: Return reference to parameter passed by reference (OK)
int& good_return_param_ref(int& param) {
  return param; // OK: parameter has external storage
}

// Test case 9: Return pointer to parameter passed by pointer (OK)
int* good_return_param_ptr(int* param) {
  return param; // OK: parameter points to external storage
}

// Test case 10: Lambda capturing local by reference (should warn)
auto bad_lambda_capture() {
  int local = 42;
  return [&local]() -> int& {
    return local; // CHECK-MESSAGES: :[[@LINE]]:12: warning: lambda returns reference to local variable 'local' captured by reference [automotive-cpp23-req-7.5.1]
    // CHECK-MESSAGES: :[[@LINE-3]]:7: note: variable declared here
  };
}

// Test case 11: Lambda capturing by value (OK)
auto good_lambda_capture() {
  int local = 42;
  return [local]() -> int {
    return local; // OK: captured by value
  };
}

// Test case 12: Multiple local variables
int* bad_multiple_locals() {
  int a = 1, b = 2, c = 3;
  int* ptr = &b;
  return ptr; // CHECK-MESSAGES: :[[@LINE]]:3: warning: function returns pointer variable 'ptr' that refers to local variable 'b' with automatic storage duration [automotive-cpp23-req-7.5.1]
  // CHECK-MESSAGES: :[[@LINE-3]]:14: note: referenced variable declared here
  // CHECK-MESSAGES: :[[@LINE-3]]:8: note: pointer variable declared here
}

// Test case 13: Return address from nested scope
int* bad_nested_scope() {
  {
    int local = 42;
    return &local; // CHECK-MESSAGES: :[[@LINE]]:5: warning: function returns address of local variable 'local' with automatic storage duration [automotive-cpp23-req-7.5.1]
    // CHECK-MESSAGES: :[[@LINE-2]]:9: note: variable declared here
  }
}

// Test case 14: Return reference to const local (still bad)
const int& bad_return_const_ref() {
  int local = 42;
  return local; // CHECK-MESSAGES: :[[@LINE]]:3: warning: function returns reference to local variable 'local' with automatic storage duration [automotive-cpp23-req-7.5.1]
  // CHECK-MESSAGES: :[[@LINE-2]]:7: note: variable declared here
}

// Test case 15: Array local variable
int* bad_return_array_element() {
  int arr[10];
  return &arr[0]; // CHECK-MESSAGES: :[[@LINE]]:3: warning: function returns address of local variable 'arr' with automatic storage duration [automotive-cpp23-req-7.5.1]
  // CHECK-MESSAGES: :[[@LINE-2]]:7: note: variable declared here
}

// Test case 16: Struct local variable
struct Point {
  int x, y;
};

Point* bad_return_struct() {
  Point p{1, 2};
  return &p; // CHECK-MESSAGES: :[[@LINE]]:3: warning: function returns address of local variable 'p' with automatic storage duration [automotive-cpp23-req-7.5.1]
  // CHECK-MESSAGES: :[[@LINE-2]]:9: note: variable declared here
}

// Test case 17: Return pointer to member of local struct
int* bad_return_member() {
  Point p{1, 2};
  return &p.x; // OK for this simple check (member access not detected by current matcher)
}

// Test case 18: Global variable (OK)
int global_var = 100;

int* good_return_global() {
  return &global_var; // OK: global storage duration
}

int& good_return_global_ref() {
  return global_var; // OK: global storage duration
}

// Test case 19: Thread-local variable (OK)
thread_local int tls_var = 200;

int* good_return_thread_local() {
  return &tls_var; // OK: thread storage duration
}

// Test case 20: Conditional return
int* bad_conditional_return(bool flag) {
  int local = 42;
  static int persistent = 99;

  if (flag)
    return &persistent; // OK: static
  else
    return &local; // CHECK-MESSAGES: :[[@LINE]]:5: warning: function returns address of local variable 'local' with automatic storage duration [automotive-cpp23-req-7.5.1]
    // CHECK-MESSAGES: :[[@LINE-5]]:7: note: variable declared here
}

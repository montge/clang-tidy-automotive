// RUN: %check_clang_tidy -std=c++11-or-later %s automotive-cpp23-adv-8.20 %t
// Test for automotive-cpp23-adv-8.20: noreturn function with non-void return type

// Test with [[noreturn]] attribute and non-void return type (C++11)
// CHECK-MESSAGES: :[[@LINE+1]]:18: warning: noreturn function 'exitWithInt' should have void return type, but has int
[[noreturn]] int exitWithInt() {
  __builtin_trap();
}

// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: noreturn function 'exitWithFloat' should have void return type, but has float
[[noreturn]] float exitWithFloat() {
  __builtin_trap();
}

// CHECK-MESSAGES: :[[@LINE+1]]:21: warning: noreturn function 'exitWithDouble' should have void return type, but has double
[[noreturn]] double exitWithDouble() {
  __builtin_trap();
}

// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: noreturn function 'exitWithChar' should have void return type, but has char
[[noreturn]] char exitWithChar() {
  __builtin_trap();
}

// Pointer return types
// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: noreturn function 'exitWithPtr' should have void return type, but has int *
[[noreturn]] int* exitWithPtr() {
  __builtin_trap();
}

// Reference return types
// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: noreturn function 'exitWithRef' should have void return type, but has int &
[[noreturn]] int& exitWithRef() {
  __builtin_trap();
}

// Struct return type
struct MyStruct {
  int x;
  int y;
};

// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: noreturn function 'exitWithStruct' should have void return type, but has MyStruct
[[noreturn]] MyStruct exitWithStruct() {
  __builtin_trap();
}

// Compliant: noreturn function with void return type
[[noreturn]] void compliantExit() {
  __builtin_trap();
}

// Test with class member function
class MyClass {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: noreturn function 'terminate' should have void return type, but has int
  [[noreturn]] int terminate() {
    __builtin_trap();
  }

  // Compliant: void return type
  [[noreturn]] void compliantTerminate() {
    __builtin_trap();
  }

  // Static member function
  // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: noreturn function 'staticTerminate' should have void return type, but has int
  [[noreturn]] static int staticTerminate() {
    __builtin_trap();
  }
};

// Note: _Noreturn (C11-style) is handled differently in C++ mode and
// may not be flagged by this check which specifically targets [[noreturn]]

// Compliant: function without noreturn attribute can have any return type
int normalFunction() {
  return 42;  // OK - not noreturn
}

double anotherNormalFunction() {
  return 3.14;  // OK - not noreturn
}

// Test with const and volatile qualifiers
// CHECK-MESSAGES: :[[@LINE+1]]:24: warning: noreturn function 'exitWithConst' should have void return type, but has const int
[[noreturn]] const int exitWithConst() {
  __builtin_trap();
}

// Test with typedef
typedef int IntType;
// CHECK-MESSAGES: :[[@LINE+1]]:22: warning: noreturn function 'exitWithTypedef' should have void return type, but has IntType (aka 'int')
[[noreturn]] IntType exitWithTypedef() {
  __builtin_trap();
}

// Test with enum
enum Color { RED, GREEN, BLUE };
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: noreturn function 'exitWithEnum' should have void return type, but has Color
[[noreturn]] Color exitWithEnum() {
  __builtin_trap();
}

// Test with noexcept specifier
// CHECK-MESSAGES: :[[@LINE+1]]:18: warning: noreturn function 'exitNoexcept' should have void return type, but has int
[[noreturn]] int exitNoexcept() noexcept {
  __builtin_trap();
}

// Compliant template with void
template<typename T>
[[noreturn]] void templateCompliant() {
  __builtin_trap();
}

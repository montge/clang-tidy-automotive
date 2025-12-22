// RUN: %check_clang_tidy -std=c++11-or-later %s automotive-cpp23-adv-8.19 %t
// Test for automotive-cpp23-adv-8.19: return in noreturn function

// Test with [[noreturn]] attribute (C++11)
[[noreturn]] void terminateWithReturn() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: return statement in noreturn function 'terminateWithReturn' violates the function's contract to never return
  return;
}

[[noreturn]] void terminateWithReturnValue() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: return statement in noreturn function 'terminateWithReturnValue' violates the function's contract to never return
  return;
}

// Compliant: noreturn function without return statement
[[noreturn]] void compliantTerminate() {
  __builtin_trap();
}

// Test with class member function
class MyClass {
public:
  [[noreturn]] void terminate() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return statement in noreturn function 'terminate' violates the function's contract to never return
    return;
  }

  [[noreturn]] void compliantTerminate() {
    __builtin_trap();
  }
};

// Compliant: function without noreturn attribute can return
void normalFunction() {
  return;  // OK - not noreturn
}

int normalFunctionWithValue() {
  return 42;  // OK - not noreturn
}

// Test nested function call
[[noreturn]] void nestedReturn() {
  if (true) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return statement in noreturn function 'nestedReturn' violates the function's contract to never return
    return;
  }
  __builtin_trap();
}

// Test multiple return statements
[[noreturn]] void multipleReturns(bool flag) {
  if (flag) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return statement in noreturn function 'multipleReturns' violates the function's contract to never return
    return;
  } else {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return statement in noreturn function 'multipleReturns' violates the function's contract to never return
    return;
  }
}

// Compliant: noreturn function with infinite loop
[[noreturn]] void infiniteLoop() {
  while (true) {
    // Do something
  }
}

// Compliant: noreturn function that throws
[[noreturn]] void throwFunc() {
  throw 42;
}

// RUN: %check_clang_tidy -std=c++11-or-later %s automotive-cpp23-req-18.4.3 %t
// Test for automotive-cpp23-req-18.4.3: throw in noexcept function

void noexceptFunc() noexcept {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throw expression in noexcept function 'noexceptFunc' will call std::terminate if exception propagates
  throw 123;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 'noexceptFunc' has a non-throwing exception specification but can still throw [clang-diagnostic-exceptions]
  // CHECK-MESSAGES: :[[@LINE-4]]:6: note: function declared non-throwing here
}

void anotherNoexceptFunc() noexcept(true) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throw expression in noexcept function 'anotherNoexceptFunc' will call std::terminate if exception propagates
  throw 42;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 'anotherNoexceptFunc' has a non-throwing exception specification but can still throw [clang-diagnostic-exceptions]
  // CHECK-MESSAGES: :[[@LINE-4]]:6: note: function declared non-throwing here
}

class MyClass {
public:
  void method() noexcept {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in noexcept function 'method' will call std::terminate if exception propagates
    throw "error";
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 'method' has a non-throwing exception specification but can still throw [clang-diagnostic-exceptions]
    // CHECK-MESSAGES: :[[@LINE-4]]:8: note: function declared non-throwing here
  }
};

// Compliant: throwing functions are not noexcept
void throwingFunc() {
  throw 999;  // OK - not noexcept
}

void mayThrow() noexcept(false) {
  throw 42;  // OK - explicitly allows throwing
}

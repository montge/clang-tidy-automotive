// RUN: %check_clang_tidy -std=c++11-or-later %s automotive-cpp23-req-18.4.3 %t
// Test for automotive-cpp23-req-18.4.3: throw in noexcept function

#include <stdexcept>

void noexceptFunc() noexcept {
  // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in noexcept function
  throw std::runtime_error("error");
}

void anotherNoexceptFunc() noexcept(true) {
  // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in noexcept function
  throw 42;
}

class MyClass {
public:
  void method() noexcept {
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: throw expression in noexcept function
    throw "error";
  }
};

// Compliant: throwing functions are not noexcept
void throwingFunc() {
  throw std::runtime_error("error");  // OK - not noexcept
}

void mayThrow() noexcept(false) {
  throw 42;  // OK - explicitly allows throwing
}

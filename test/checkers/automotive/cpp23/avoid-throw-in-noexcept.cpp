// RUN: %check_clang_tidy -std=c++11-or-later %s automotive-cpp23-req-18.4.3 %t
// Test for automotive-cpp23-req-18.4.3: throw in noexcept function

void noexceptFunc() noexcept {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throw expression in noexcept function 'noexceptFunc' will call std::terminate if exception propagates
  throw 123;
}

void anotherNoexceptFunc() noexcept(true) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throw expression in noexcept function 'anotherNoexceptFunc' will call std::terminate if exception propagates
  throw 42;
}

class MyClass {
public:
  void method() noexcept {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in noexcept function 'method' will call std::terminate if exception propagates
    throw "error";
  }
};

// Compliant: throwing functions are not noexcept
void throwingFunc() {
  throw 999;  // OK - not noexcept
}

void mayThrow() noexcept(false) {
  throw 42;  // OK - explicitly allows throwing
}

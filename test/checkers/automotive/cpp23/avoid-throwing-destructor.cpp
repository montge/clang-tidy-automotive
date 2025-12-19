// RUN: %check_clang_tidy %s automotive-cpp23-req-18.4.1 %t
// Test for automotive-cpp23-req-18.4.1: Destructors shall not throw

#include <stdexcept>

class BadDestructor {
public:
  ~BadDestructor() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in destructor
    throw std::runtime_error("error in destructor");
  }
};

class GoodDestructor {
public:
  ~GoodDestructor() noexcept {
    // No throw - compliant
  }
};

class ImplicitNoexcept {
public:
  // In C++11+, destructors are implicitly noexcept
  ~ImplicitNoexcept() {
    // No throw - compliant
  }
};

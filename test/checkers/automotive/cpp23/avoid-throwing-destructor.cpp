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

class AnotherBadDestructor {
public:
  ~AnotherBadDestructor() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in destructor
    throw 42;
  }
};

// Destructor with explicit noexcept(false) - should warn
class ExplicitThrowingDestructor {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor {{.*}} has exception specification that may throw
  ~ExplicitThrowingDestructor() noexcept(false) {
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

// Defaulted destructor - should not warn
class DefaultedDestructor {
public:
  ~DefaultedDestructor() = default;
};

// Deleted destructor - should not warn
class DeletedDestructor {
public:
  ~DeletedDestructor() = delete;
};

// RUN: %check_clang_tidy %s automotive-cpp23-req-18.2 %t
// Test for automotive-cpp23-req-18.2: Exceptions shall not propagate out of destructors
// This is an alias for Rule 18.4.1

class BadDestructor {
public:
  ~BadDestructor() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in destructor '~BadDestructor'; destructors should not throw exceptions
    throw 999;
  }
};

class AnotherBadDestructor {
public:
  ~AnotherBadDestructor() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throw expression in destructor '~AnotherBadDestructor'; destructors should not throw exceptions
    throw 42;
  }
};

// Destructor with explicit noexcept(false) - should warn
class ExplicitThrowingDestructor {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: destructor '~ExplicitThrowingDestructor' has exception specification that may throw; destructors should be noexcept
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

// RUN: %check_clang_tidy %s automotive-cpp23-adv-18.3.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-18.3.1: exceptions shall derive from std::exception

// Mock std::exception for testing (without including <exception>)
namespace std {
  class exception {
  public:
    virtual ~exception() {}
    virtual const char* what() const noexcept { return ""; }
  };
}

class BadException {};

// OK - derives from std::exception
class GoodException : public std::exception {
  const char* what() const noexcept override { return "good"; }
};

void throw_bad() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing class 'BadException' that does not derive from std::exception is not allowed (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw BadException();
}

void throw_good() {
  throw GoodException();
}

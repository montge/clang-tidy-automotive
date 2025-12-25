// RUN: %check_clang_tidy %s automotive-cpp23-adv-18.3.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-18.3.1: exceptions shall derive from std::exception

#include <exception>

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: exception class 'BadException' should derive from std::exception
class BadException {};

// OK - derives from std::exception
class GoodException : public std::exception {
  const char* what() const noexcept override { return "good"; }
};

void throw_bad() {
  throw BadException();
}

void throw_good() {
  throw GoodException();
}

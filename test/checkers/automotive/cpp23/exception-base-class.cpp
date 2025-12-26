// RUN: %check_clang_tidy %s automotive-cpp23-adv-18.3.1 %t

// Mock std::exception for testing (without including <exception>)
namespace std {
  class exception {
  public:
    exception() noexcept {}
    virtual ~exception() {}
    virtual const char* what() const noexcept { return "exception"; }
  };

  class runtime_error : public exception {
  public:
    explicit runtime_error(const char*) {}
  };

  class logic_error : public exception {
  public:
    explicit logic_error(const char*) {}
  };
}

// Compliant: exception derived from std::exception
class MyException : public std::exception {
public:
  const char* what() const noexcept { return "MyException"; }
};

// Compliant: derived from std::runtime_error
class MyRuntimeError : public std::runtime_error {
public:
  MyRuntimeError() : std::runtime_error("error") {}
};

// Non-compliant: does not derive from std::exception
class BadException {
public:
  const char* what() const { return "BadException"; }
};

// Non-compliant: does not derive from std::exception
class AnotherBadException {};

void test_primitive_types() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing primitive type 'int' is not allowed; exceptions must derive from std::exception (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing primitive type 'long' is not allowed; exceptions must derive from std::exception (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw 42L;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing primitive type 'double' is not allowed; exceptions must derive from std::exception (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw 3.14;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing primitive type 'bool' is not allowed; exceptions must derive from std::exception (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw true;
}

void test_string_literals() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing C-string literal is not allowed; exceptions must derive from std::exception (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw "error message";

  const char* msg = "error";
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing pointer type 'const char *' is not allowed; exceptions must derive from std::exception (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw msg;
}

void test_bad_classes() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing class 'BadException' that does not derive from std::exception is not allowed (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw BadException();

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing class 'AnotherBadException' that does not derive from std::exception is not allowed (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw AnotherBadException();

  BadException e;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing class 'BadException' that does not derive from std::exception is not allowed (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw e;
}

void test_compliant_exceptions() {
  // OK: derives from std::exception
  throw MyException();

  // OK: derives from std::runtime_error which derives from std::exception
  throw MyRuntimeError();

  // OK: std::runtime_error
  throw std::runtime_error("error");

  // OK: std::logic_error
  throw std::logic_error("logic error");

  // OK: std::exception itself
  throw std::exception();

  MyException e;
  // OK: derives from std::exception
  throw e;
}

void test_rethrow() {
  try {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throwing class 'BadException' that does not derive from std::exception is not allowed (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
    throw BadException();
  } catch (...) {
    // OK: re-throw is allowed (throw without operand)
    throw;
  }
}

void test_pointer_exceptions() {
  BadException* e = new BadException();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing pointer type 'BadException *' is not allowed; exceptions must derive from std::exception (MISRA C++:2023 Rule 18.3.1) [automotive-cpp23-adv-18.3.1]
  throw e;

  MyException* me = new MyException();
  // OK: pointer to type derived from std::exception
  // Note: throwing pointers is generally not recommended in C++, but this check
  // only verifies the base class requirement
  throw me;
}

// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-adv-10.6.1 %t

// Test MISRA C++:2023 Rule 10.6.1 - Functions that return values that should not be
// ignored shall be declared with the [[nodiscard]] attribute

#include <memory>
#include <optional>

// ===== Boolean return types (error indicators) =====

// Non-compliant - public function returning bool without [[nodiscard]]
bool validate();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns boolean (potential error indicator) but is not marked [[nodiscard]]; functions that return values that should not be ignored shall be declared with the [[nodiscard]] attribute (MISRA C++:2023 Rule 10.6.1) [automotive-cpp23-adv-10.6.1]

// Compliant - marked with [[nodiscard]]
[[nodiscard]] bool validate_good();

// ===== Pointer return types (potential null on failure) =====

// Non-compliant - returns pointer
int* allocate();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns pointer (potential null on failure) but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] int* allocate_good();

// ===== Integer return types (error codes) =====

// Non-compliant - returns int (potential error code)
int open_file();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns integer (potential error code) but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] int open_file_good();

// Non-compliant - returns unsigned (potential error code)
unsigned int get_status();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns integer (potential error code) but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] unsigned int get_status_good();

// ===== Enumeration return types (error codes) =====

enum class ErrorCode {
  Success,
  Failure
};

// Non-compliant - returns enum (potential error code)
ErrorCode process();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns enumeration (potential error code) but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] ErrorCode process_good();

// ===== Smart pointers and optionals =====

// Non-compliant - returns std::optional
std::optional<int> find_value();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns value that should not be ignored but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] std::optional<int> find_value_good();

// Non-compliant - returns unique_ptr
std::unique_ptr<int> create();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns value that should not be ignored but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] std::unique_ptr<int> create_good();

// ===== Functions that should NOT be flagged =====

// OK - returns void
void do_something();

// OK - returns void
void process_data();

// ===== Class member functions =====

class PublicInterface {
public:
  // Non-compliant - public member function returning bool
  bool check();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function returns boolean (potential error indicator) but is not marked [[nodiscard]]

  // Compliant
  [[nodiscard]] bool check_good();

  // OK - returns void
  void set_value(int x);

  // Non-compliant - public member function returning pointer
  int* get_data();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function returns pointer (potential null on failure) but is not marked [[nodiscard]]

  // Compliant
  [[nodiscard]] int* get_data_good();

private:
  // OK by default - private function (not checked unless CheckPrivateFunctions is enabled)
  bool internal_check();

  // OK - private void function
  void internal_process();

protected:
  // OK by default - protected function (not checked unless CheckPrivateFunctions is enabled)
  bool protected_check();
};

// ===== Operators should NOT be flagged =====

class OperatorTest {
public:
  // OK - operator= should not require [[nodiscard]]
  OperatorTest& operator=(const OperatorTest& other);

  // OK - conversion operator
  operator bool() const;

  // OK - operator[]
  int& operator[](int index);
};

// ===== Constructors and destructors should NOT be flagged =====

class ConstructorTest {
public:
  // OK - constructor (not flagged even though it's implicit conversion)
  ConstructorTest(int x);

  // OK - destructor
  ~ConstructorTest();
};

// ===== Static functions =====

// Non-compliant - free function with internal linkage still should be checked at declaration
static bool static_validate();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns boolean (potential error indicator) but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] static bool static_validate_good();

// ===== Function definitions =====

// Non-compliant - function declaration
bool function_decl();
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: function returns boolean (potential error indicator) but is not marked [[nodiscard]]

// Compliant
[[nodiscard]] bool function_decl_good();

// Definition (should not double-warn)
bool function_decl() {
  return true;
}

// ===== Reference return types =====

class RefTest {
public:
  // Non-compliant - returns pointer by reference
  int*& get_ref();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function returns pointer (potential null on failure) but is not marked [[nodiscard]]

  // Compliant
  [[nodiscard]] int*& get_ref_good();

  // OK - returns reference to int (not a pointer/bool/error code)
  int& get_value();
};

// ===== Character types should NOT be flagged =====

// OK - char is not typically an error code
char get_char();

// OK - unsigned char
unsigned char get_byte();

// ===== Deleted and defaulted functions =====

class DefaultedTest {
public:
  // OK - defaulted constructor
  DefaultedTest() = default;

  // OK - defaulted destructor
  ~DefaultedTest() = default;

  // OK - deleted function
  bool deleted_function() = delete;
};

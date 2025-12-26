// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-adv-10.6.1 %t

// Test MISRA C++:2023 Rule 10.6.1 - Functions that return values that should not be
// ignored shall be declared with the [[nodiscard]] attribute

// ===== Boolean return types (error indicators) =====

// Non-compliant - public function returning bool without [[nodiscard]]
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function returns boolean (potential error indicator) but is not marked {{\[\[}}nodiscard{{\]\]}}
bool validate();

// Compliant - marked with [[nodiscard]]
[[nodiscard]] bool validate_good();

// ===== Pointer return types (potential null on failure) =====

// Non-compliant - returns pointer
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function returns pointer (potential null on failure) but is not marked {{\[\[}}nodiscard{{\]\]}}
int* allocate();

// Compliant
[[nodiscard]] int* allocate_good();

// ===== Integer return types (error codes) =====

// Non-compliant - returns int (potential error code)
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function returns integer (potential error code) but is not marked {{\[\[}}nodiscard{{\]\]}}
int open_file();

// Compliant
[[nodiscard]] int open_file_good();

// Non-compliant - returns unsigned (potential error code)
// CHECK-MESSAGES: :[[@LINE+1]]:14: warning: function returns integer (potential error code) but is not marked {{\[\[}}nodiscard{{\]\]}}
unsigned int get_status();

// Compliant
[[nodiscard]] unsigned int get_status_good();

// ===== Compliant cases (void return, setters, etc.) =====

// Compliant - returns void (nothing to discard)
void do_something();

// Compliant - setter typically not nodiscard
void set_value(int x);

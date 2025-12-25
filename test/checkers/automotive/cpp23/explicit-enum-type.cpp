// RUN: %check_clang_tidy %s automotive-cpp23-req-10.2.1 %t

// Test: Enumeration underlying type shall be explicitly specified

// Use built-in types instead of cstdint for testing
typedef unsigned char byte;

// Violations - old-style enum without explicit underlying type
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: enumeration underlying type shall be explicitly specified
enum Color { Red, Green, Blue };

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: enumeration underlying type shall be explicitly specified
enum Status { OK, Error };

// Violations - scoped enum without explicit underlying type
// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: enumeration underlying type shall be explicitly specified
enum class TrafficLight { Red, Yellow, Green };

// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: enumeration underlying type shall be explicitly specified
enum class ErrorCode { Success, Failure };

// Compliant - old-style enum with explicit underlying type
enum Priority : int { Low, Medium, High };

enum Size : unsigned int { Small, Large };

enum Index : unsigned { First = 0, Second = 1 };

enum Level : long { Min = -100, Max = 100 };

// Compliant - scoped enum with explicit underlying type
enum class Temperature : int { Cold, Warm, Hot };

enum class FileMode : unsigned char { Read, Write, Execute };

enum class State : byte { Idle, Running, Stopped };

enum class Direction : short { North, South, East, West };

// Compliant - using type aliases
enum class ByteCode : byte { Start = 0x00, End = 0xFF };

// Edge cases - forward declarations
enum ForwardEnum : int;  // OK - explicit type
// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: enumeration underlying type shall be explicitly specified
enum class ForwardScoped;  // Warning - no explicit type

// Compliant - definition matching forward declaration
enum ForwardEnum : int { A, B, C };

// Nested in class
class Container {
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: enumeration underlying type shall be explicitly specified
  enum Inner { X, Y, Z };

  enum Outer : int { P, Q, R };  // OK

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: enumeration underlying type shall be explicitly specified
  enum class Scoped { Alpha, Beta };

  enum class Fixed : unsigned { Gamma, Delta };  // OK
};

// Empty enums
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: enumeration underlying type shall be explicitly specified
enum Empty {};

enum EmptyFixed : int {};  // OK

// With attributes (if supported)
enum [[maybe_unused]] Tagged : int { T1, T2 };  // OK

// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: enumeration underlying type shall be explicitly specified
enum [[maybe_unused]] Untyped { U1, U2 };

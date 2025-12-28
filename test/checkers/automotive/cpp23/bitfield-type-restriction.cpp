// RUN: %check_clang_tidy %s automotive-cpp23-req-12.2 %t

// MISRA C++:2023 Rule 12.2 - Bit-field types shall be restricted to bool or unsigned integer types

// Mock type definitions
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

struct NonCompliant {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: bit-field type shall be bool or unsigned integer type; found 'int' (MISRA C++:2023 Rule 12.2)
  int x : 5;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: bit-field type shall be bool or unsigned integer type; found 'char' (MISRA C++:2023 Rule 12.2)
  char c : 3;

  // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: bit-field type shall be bool or unsigned integer type; found 'short' (MISRA C++:2023 Rule 12.2)
  short s : 4;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: bit-field type shall be bool or unsigned integer type; found 'long' (MISRA C++:2023 Rule 12.2)
  long l : 10;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bit-field type shall be bool or unsigned integer type; found 'long long' (MISRA C++:2023 Rule 12.2)
  long long ll : 20;
};

struct Compliant {
  unsigned int x : 5;       // OK
  bool flag : 1;            // OK
  uint8_t y : 3;            // OK
  uint16_t z : 12;          // OK
  unsigned char c : 4;      // OK
  unsigned short s : 6;     // OK
  unsigned long l : 16;     // OK
  unsigned long long ll : 32; // OK
};

// Const qualified types
struct ConstTypes {
  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bit-field type shall be bool or unsigned integer type; found 'const int' (MISRA C++:2023 Rule 12.2)
  const int x : 5;

  const unsigned int y : 5;  // OK - unsigned even with const
  const bool flag : 1;       // OK - bool with const
};

// Enum types
enum class StrongColor { RED, GREEN, BLUE };

struct EnumBitfields {
  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: bit-field type shall be bool or unsigned integer type; found 'StrongColor' (MISRA C++:2023 Rule 12.2)
  StrongColor sc : 2;
};

// RUN: %check_clang_tidy %s automotive-cpp23-req-5.0 %t

// MISRA C++:2023 Rule 5.0: A type-specifier-seq or type-id shall not define
// any types. Types should be defined separately, not inline in declarations.

// Non-compliant: anonymous struct defined inline in variable declaration
struct { int x; int y; } point1;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: type defined inline in variable declaration; define the type separately

// Non-compliant: anonymous enum defined inline
enum { RED, GREEN, BLUE } color1;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: type defined inline in variable declaration; define the type separately

// Compliant: named type defined separately
struct Point {
  int x;
  int y;
};

Point point2;

// Compliant: named enum
enum Color { RED2, GREEN2, BLUE2 };
Color color2;

class Container {
  // Non-compliant: anonymous struct in field declaration
  struct { int value; } data;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: type defined inline in field declaration; define the type separately
};

// Non-compliant: typedef with inline struct definition
typedef struct { int a; int b; } InlinePair;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: type defined inline in typedef; define the type separately

// Compliant: typedef referencing named struct
struct NamedPair { int a; int b; };
typedef struct NamedPair AnotherPair;

// Non-compliant: using alias with inline definition
using InlineTriple = struct { int x; int y; int z; };
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: type defined inline in type alias; define the type separately

// Compliant: pointer to named type
Point* pointPtr;

// Non-compliant: pointer to anonymous struct
struct { int a; }* anonPtr;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: type defined inline in variable declaration; define the type separately

// Non-compliant: anonymous union in variable
union { int i; float f; } mixedData;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: type defined inline in variable declaration; define the type separately

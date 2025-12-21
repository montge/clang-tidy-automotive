// Test file for: automotive-c23-req-5.7
// Related MISRA C:2025 Rule: 5.7 - A tag name shall be a unique identifier
//
// NOTE: Testing violations is not possible because duplicate struct/union/enum
// definitions are compile errors in C. This test verifies that compliant
// code (unique tag names, forward declarations) does NOT trigger warnings.

// RUN: clang-tidy %s --checks='-*,automotive-c23-req-5.7' -- 2>&1 | FileCheck %s -allow-empty
// CHECK-NOT: warning:

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Unique struct names
struct Point {
    int x;
    int y;
};

struct Rectangle {
    int width;
    int height;
};

struct Circle {
    int radius;
    int centerX;
    int centerY;
};

// Unique union names
union IntOrFloat {
    int i;
    float f;
};

union CharOrInt {
    char c;
    int i;
};

// Unique enum names
enum Color {
    RED,
    GREEN,
    BLUE
};

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

enum Priority {
    LOW,
    MEDIUM,
    HIGH
};

// Forward declaration and definition of same tag (NOT duplicate)
struct Node;  // Forward declaration

struct Node {  // Definition - should NOT warn
    int data;
    struct Node *next;
};

// Another forward declaration pattern
struct Forward;
void use_forward(struct Forward *ptr);
struct Forward {  // Definition - should NOT warn
    int value;
};

// Typedef doesn't create duplicate
struct Element {
    int id;
};

typedef struct Element Element_t;

// Anonymous structs/unions (no name, so no duplicate)
struct Outer {
    struct {
        int a;
        int b;
    } inner1;

    struct {
        int c;
        int d;
    } inner2;
};

// Using tag in typedef without creating duplicate
struct Container {
    int capacity;
};

typedef struct Container* ContainerPtr;

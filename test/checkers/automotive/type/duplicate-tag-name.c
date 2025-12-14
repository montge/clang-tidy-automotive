// Test file for: automotive-duplicate-tag-name
// Related MISRA C:2025 Rule: 5.7
//
// This file tests the detection of duplicate tag names (struct, union, enum)

// RUN: %check_clang_tidy %s automotive-duplicate-tag-name %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Duplicate struct names
struct Point {
    int x;
    int y;
};

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: duplicate tag name 'Point'; struct 'Point' conflicts with struct declared here [automotive-duplicate-tag-name]
struct Point {
    float x;
    float y;
};

// Duplicate union names
union Data {
    int i;
    float f;
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: duplicate tag name 'Data'; union 'Data' conflicts with union declared here [automotive-duplicate-tag-name]
union Data {
    char c;
    double d;
};

// Duplicate enum names
enum Color {
    RED,
    GREEN,
    BLUE
};

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: duplicate tag name 'Color'; enum 'Color' conflicts with enum declared here [automotive-duplicate-tag-name]
enum Color {
    CYAN,
    MAGENTA,
    YELLOW
};

// Different kinds with same name (struct vs enum)
struct Status {
    int code;
    char message[100];
};

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: duplicate tag name 'Status'; enum 'Status' conflicts with struct declared here [automotive-duplicate-tag-name]
enum Status {
    OK,
    ERROR,
    PENDING
};

// Union vs struct with same name
union Value {
    int i;
    float f;
};

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: duplicate tag name 'Value'; struct 'Value' conflicts with union declared here [automotive-duplicate-tag-name]
struct Value {
    int type;
    int data;
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Unique struct names
struct Rectangle {
    int width;
    int height;
};

struct Circle {
    int radius;
    int x;
    int y;
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

// Typedef doesn't create duplicate
struct Element {
    int id;
};

typedef struct Element Element_t;  // This is OK

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

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

// Tags in different scopes (function scope)
void func1(void) {
    struct Local {
        int x;
    };
}

void func2(void) {
    // In C, tag names have file scope, not block scope
    // So this would be a duplicate of the one in func1
    // However, some compilers may allow this
    struct Local {
        int y;
    };
}

// Using tag in typedef without creating duplicate
struct Container {
    int capacity;
};

typedef struct Container* ContainerPtr;  // OK, not duplicate

// Incomplete type followed by complete type (NOT duplicate)
struct Forward;  // Incomplete declaration

void use_forward(struct Forward *ptr);

struct Forward {  // Complete definition - should NOT warn
    int value;
};

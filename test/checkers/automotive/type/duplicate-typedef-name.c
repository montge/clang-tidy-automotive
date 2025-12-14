// Test file for: automotive-duplicate-typedef-name
// Related MISRA C:2025 Rule: 5.6
//
// This file tests the detection of duplicate typedef names

// RUN: %check_clang_tidy %s automotive-duplicate-typedef-name %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Simple duplicate typedef
typedef int MyInt;

// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: duplicate typedef name 'MyInt', previously declared [automotive-duplicate-typedef-name]
typedef int MyInt;

// Duplicate typedef with different underlying types
typedef char* StringPtr;

// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: duplicate typedef name 'StringPtr', previously declared [automotive-duplicate-typedef-name]
typedef const char* StringPtr;

// Duplicate typedef for struct
typedef struct {
    int x;
    int y;
} Point;

// CHECK-MESSAGES: :[[@LINE+1]]:3: warning: duplicate typedef name 'Point', previously declared [automotive-duplicate-typedef-name]
} Point;

// Duplicate function pointer typedef
typedef int (*FuncPtr)(int);

// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: duplicate typedef name 'FuncPtr', previously declared [automotive-duplicate-typedef-name]
typedef void (*FuncPtr)(void);

// Duplicate array typedef
typedef int IntArray[10];

// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: duplicate typedef name 'IntArray', previously declared [automotive-duplicate-typedef-name]
typedef int IntArray[20];

// Duplicate typedef for enum
typedef enum {
    VAL1,
    VAL2
} EnumType;

// CHECK-MESSAGES: :[[@LINE+1]]:3: warning: duplicate typedef name 'EnumType', previously declared [automotive-duplicate-typedef-name]
} EnumType;

// Duplicate with unsigned/signed variation
typedef unsigned int UInt;

// CHECK-MESSAGES: :[[@LINE+1]]:18: warning: duplicate typedef name 'UInt', previously declared [automotive-duplicate-typedef-name]
typedef unsigned UInt;

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Unique typedef names
typedef int Integer;
typedef long LongInt;
typedef char Character;
typedef float Real;
typedef double Double;

// Typedef for struct with unique names
typedef struct {
    int a;
    int b;
} StructA;

typedef struct {
    int c;
    int d;
} StructB;

// Typedef for union
typedef union {
    int i;
    float f;
} UnionType;

// Typedef for enum with unique name
typedef enum {
    FIRST,
    SECOND,
    THIRD
} EnumValues;

// Function pointer typedefs with unique names
typedef int (*IntFunc)(int, int);
typedef void (*VoidFunc)(void);
typedef char* (*StringFunc)(const char*);

// Pointer typedefs with unique names
typedef int* IntPtr;
typedef char* CharPtr;
typedef void* VoidPtr;

// Array typedefs with unique names
typedef int Matrix[10][10];
typedef char Buffer[256];

// Typedef for const types
typedef const int ConstInt;
typedef const char* ConstCharPtr;

// Typedef for volatile types
typedef volatile int VolatileInt;

// Complex typedef
typedef struct Node* NodePtr;
struct Node {
    int data;
    NodePtr next;
};

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

// Using typedef in different contexts (but still unique names)
typedef struct TagName {
    int value;
} TypedefName;  // Tag name and typedef name are different - OK

// Multiple typedefs to same underlying type (but different names) - OK
typedef int Int32;
typedef int Integer32;

// Typedef with storage class specifiers
typedef struct {
    int data;
} StaticData;

static StaticData global_data;  // Using typedef - not a duplicate

// Typedef in function scope (still has file scope for typedefs in C)
void function1(void) {
    typedef int LocalInt;
}

// This would be detected as duplicate if it has the same name
void function2(void) {
    typedef long OtherLocalInt;  // Different name - OK
}

// Nested typedef usage
typedef struct Outer_s {
    int x;
    typedef struct Inner_s {  // This is actually invalid C - for edge case testing
        int y;
    } Inner;
} Outer;

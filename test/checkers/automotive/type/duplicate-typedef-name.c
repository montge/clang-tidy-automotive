// RUN: %check_clang_tidy %s automotive-c23-req-5.6 %t

// Test: Duplicate typedef names (MISRA Rule 5.6)

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Simple duplicate typedef (same type - allowed in C but flagged by MISRA)
typedef int MyInt;

// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: duplicate typedef name 'MyInt', previously declared [automotive-c23-req-5.6]
typedef int MyInt;

// Duplicate typedef for char pointer
typedef char* StringPtr;

// CHECK-MESSAGES: :[[@LINE+1]]:15: warning: duplicate typedef name 'StringPtr', previously declared [automotive-c23-req-5.6]
typedef char* StringPtr;

// Duplicate typedef for struct
struct DataStruct {
    int x;
    int y;
};
typedef struct DataStruct Point;

// CHECK-MESSAGES: :[[@LINE+1]]:27: warning: duplicate typedef name 'Point', previously declared [automotive-c23-req-5.6]
typedef struct DataStruct Point;

// Duplicate function pointer typedef
typedef int (*FuncPtr)(int);

// CHECK-MESSAGES: :[[@LINE+1]]:15: warning: duplicate typedef name 'FuncPtr', previously declared [automotive-c23-req-5.6]
typedef int (*FuncPtr)(int);

// Duplicate array typedef
typedef int IntArray[10];

// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: duplicate typedef name 'IntArray', previously declared [automotive-c23-req-5.6]
typedef int IntArray[10];

// Duplicate unsigned typedef
typedef unsigned int UInt;

// CHECK-MESSAGES: :[[@LINE+1]]:22: warning: duplicate typedef name 'UInt', previously declared [automotive-c23-req-5.6]
typedef unsigned int UInt;

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Unique typedef names
typedef int Integer;
typedef long LongInt;
typedef char Character;
typedef float Real;
typedef double Double;

// Typedef for struct with unique name
typedef struct {
    int a;
    int b;
} StructA;

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

// Pointer typedefs with unique names
typedef int* IntPtr;
typedef void* VoidPtr;

// Complex typedef
struct Node {
    int data;
    struct Node *next;
};
typedef struct Node* NodePtr;

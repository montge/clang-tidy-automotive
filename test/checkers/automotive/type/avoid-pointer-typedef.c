// RUN: %check_clang_tidy %s automotive-c23-adv-dir-4.8 %t

// Test: Pointer types should not be hidden in typedefs (MISRA Dir 4.8)

// Violation - typedef hiding pointer
typedef int *IntPtr;
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: typedef 'IntPtr' hides pointer type

typedef char *StringType;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: typedef 'StringType' hides pointer type

typedef struct Data *DataHandle;
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: typedef 'DataHandle' hides pointer type

typedef const int *ConstIntPtr;
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: typedef 'ConstIntPtr' hides pointer type

typedef void *OpaqueHandle;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: typedef 'OpaqueHandle' hides pointer type

// Compliant - function pointer typedefs (acceptable)
typedef void (*FunctionPtr)(int);
typedef int (*Comparator)(const void *, const void *);

// Compliant - non-pointer typedefs
typedef int Integer;
typedef unsigned long Size;
typedef struct { int x; int y; } Point;

// Compliant - array typedef (not a pointer)
typedef int IntArray[10];

// Compliant - struct typedef
struct Data {
    int value;
};
typedef struct Data DataStruct;

// Use types to avoid unused warnings
void use_types(void) {
    IntPtr ip = (IntPtr)0;
    StringType st = (StringType)0;
    DataHandle dh = (DataHandle)0;
    ConstIntPtr cip = (ConstIntPtr)0;
    OpaqueHandle oh = (OpaqueHandle)0;
    FunctionPtr fp = (FunctionPtr)0;
    Comparator cmp = (Comparator)0;
    Integer i = 0;
    Size s = 0;
    Point p = {0, 0};
    IntArray arr = {0};
    DataStruct ds = {0};
    (void)ip; (void)st; (void)dh; (void)cip; (void)oh;
    (void)fp; (void)cmp; (void)i; (void)s; (void)p;
    (void)arr; (void)ds;
}

// RUN: %check_clang_tidy -std=c++17 %s automotive-c23-adv-dir-4.8 %t

// Test: Pointer types should not be hidden in typedefs or type aliases (MISRA Dir 4.8)
// This tests C++ type aliases (using declarations)

// ============= Violations: Type aliases hiding pointers =============

// Violation - using declaration hiding pointer
using IntPtr = int*;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: type alias 'IntPtr' hides pointer type

using StringType = char*;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: type alias 'StringType' hides pointer type

using ConstIntPtr = const int*;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: type alias 'ConstIntPtr' hides pointer type

using VoidPtr = void*;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: type alias 'VoidPtr' hides pointer type

struct Data { int value; };
using DataPtr = Data*;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: type alias 'DataPtr' hides pointer type

// ============= Violations: Traditional typedefs hiding pointers =============

// Violation - typedef hiding pointer
typedef double* DoublePtr;
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: typedef 'DoublePtr' hides pointer type

typedef const char* CString;
// CHECK-MESSAGES: :[[@LINE-1]]:21: warning: typedef 'CString' hides pointer type

// ============= Compliant: Function pointers =============

// Compliant - function pointer type alias (acceptable)
using FunctionPtr = void(*)(int);
using Comparator = int(*)(const void*, const void*);
using Callback = void(*)();

// Compliant - function pointer typedef
typedef int (*CompareFunc)(int, int);

// ============= Compliant: Non-pointer types =============

// Compliant - value type aliases
using Integer = int;
using Size = unsigned long;
using Byte = unsigned char;

// Compliant - struct type alias
using DataStruct = Data;

// Compliant - reference type (not a pointer)
using IntRef = int&;
using ConstIntRef = const int&;

// Compliant - array type
using IntArray = int[10];

// Use types to avoid unused warnings
void use_types() {
    IntPtr ip = nullptr;
    StringType st = nullptr;
    ConstIntPtr cip = nullptr;
    VoidPtr vp = nullptr;
    DataPtr dp = nullptr;
    DoublePtr dblp = nullptr;
    CString cs = nullptr;
    FunctionPtr fp = nullptr;
    Comparator cmp = nullptr;
    Callback cb = nullptr;
    CompareFunc cf = nullptr;
    Integer i = 0;
    Size s = 0;
    Byte b = 0;
    DataStruct ds = {0};
    int x = 0;
    IntRef ir = x;
    ConstIntRef cir = x;
    IntArray arr = {0};
    (void)ip; (void)st; (void)cip; (void)vp; (void)dp;
    (void)dblp; (void)cs; (void)fp; (void)cmp; (void)cb;
    (void)cf; (void)i; (void)s; (void)b; (void)ds;
    (void)ir; (void)cir; (void)arr;
}

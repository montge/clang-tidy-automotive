// RUN: %check_clang_tidy %s automotive-c23-adv-1.2 %t -- -- -std=gnu11

// Test: Language extensions detection (MISRA Rule 1.2)

// Violation - GNU statement expression
int test_stmt_expr(void) {
    int result = ({ int x = 5; x * 2; });
    // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: statement expression is a GCC/Clang extension; avoid for portability [automotive-c23-adv-1.2]
    return result;
}

// Violation - __attribute__((aligned))
struct __attribute__((aligned(16))) AlignedStruct {
// CHECK-MESSAGES: :[[@LINE-1]]:37: warning: __attribute__((aligned)) is a compiler extension; use _Alignas in C11 or alignas in C++11 for portability [automotive-c23-adv-1.2]
    int x;
    int y;
};

// Violation - __attribute__((packed))
struct __attribute__((packed)) PackedStruct {
// CHECK-MESSAGES: :[[@LINE-1]]:32: warning: __attribute__((packed)) is a compiler extension and not portable [automotive-c23-adv-1.2]
    char a;
    int b;
};

// Note: alignas triggers warning (implementation expands to __attribute__)
#include <stdalign.h>
struct PortableAligned {
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: __attribute__((aligned)) is a compiler extension; use _Alignas in C11 or alignas in C++11 for portability [automotive-c23-adv-1.2]
    alignas(16) int x;
    int y;
};

// Compliant - regular struct without extensions
struct RegularStruct {
    int a;
    int b;
    int c;
};

// Compliant - regular function
int regular_function(int x) {
    return x * 2;
}

// Use types to avoid unused warnings
void use_types(void) {
    struct AlignedStruct as;
    struct PackedStruct ps;
    struct PortableAligned pa;
    struct RegularStruct rs;
    (void)as;
    (void)ps;
    (void)pa;
    (void)rs;
}

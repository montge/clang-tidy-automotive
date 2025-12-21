// RUN: %check_clang_tidy %s automotive-avoid-multiple-hash-operators %t

// Test: Multiple # or ## operators in macro definition (MISRA Rule 20.11)

// Violation - multiple ## operators
// CHECK-MESSAGES: :[[@LINE+1]]:33: warning: avoid '##' operator after '##' operator [automotive-avoid-multiple-hash-operators]
#define CONCAT3(a, b, c) a ## b ## c

// Violation - multiple # operators
// CHECK-MESSAGES: :[[@LINE+1]]:29: warning: avoid '#' operator after '#' operator [automotive-avoid-multiple-hash-operators]
#define STRINGIFY2(a, b) #a #b

// Violation - mix of # and ##
// CHECK-MESSAGES: :[[@LINE+1]]:24: warning: avoid '##' operator after '#' operator [automotive-avoid-multiple-hash-operators]
#define MIXED(a, b) #a ## b

// Compliant - single ## operator
#define CONCAT2(a, b) a ## b

// Compliant - single # operator
#define STRINGIFY1(a) #a

// Compliant - no stringify/concat operators
#define ADD(a, b) ((a) + (b))

// Compliant - nested macro calls (each uses single operator)
#define INNER(x) #x
#define OUTER(x) INNER(x)

// Use macros to avoid unused warnings
int main(void) {
    int ab = 1;
    int abc = 2;
    (void)CONCAT2(a, b);
    (void)CONCAT3(a, b, c);
    const char* s1 = STRINGIFY1(hello);
    const char* s2 = STRINGIFY2(hello, world);
    (void)s1;
    (void)s2;
    return ADD(1, 2);
}

// MISRA C:2025 Rule 12.3 - The comma operator should not be used
// Category: Advisory
// Status: VIOLATION
//
// This code violates Rule 12.3 because it uses the comma operator.
// The comma operator can make code confusing and error-prone by
// hiding multiple operations in a single expression.
//
// Expected diagnostic:
//   warning: comma operator used in expression
//   [automotive-avoid-comma-operator]

#include <stdio.h>

int main(void) {
    int a, b, c;

    // VIOLATION: Comma operator in initialization
    a = 1, b = 2, c = 3;

    // VIOLATION: Comma operator in for loop
    for (int i = 0, j = 10; i < j; i++, j--) {
        printf("i=%d, j=%d\n", i, j);
    }

    // VIOLATION: Comma operator in expression
    int result = (a = 5, b = 10, a + b);
    printf("Result: %d\n", result);

    return 0;
}

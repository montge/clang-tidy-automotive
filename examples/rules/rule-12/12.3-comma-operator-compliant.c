// MISRA C:2025 Rule 12.3 - The comma operator should not be used
// Category: Advisory
// Status: COMPLIANT
//
// This code complies with Rule 12.3 by using separate statements
// instead of the comma operator. Each operation is clearly visible.

#include <stdio.h>

int main(void) {
    int a;
    int b;
    int c;

    // Separate statements instead of comma operator
    a = 1;
    b = 2;
    c = 3;

    // Use while loop to avoid comma in for loop increment
    int i = 0;
    int j = 10;
    while (i < j) {
        printf("i=%d, j=%d\n", i, j);
        i++;
        j--;
    }

    // Separate statements for complex operations
    a = 5;
    b = 10;
    int result = a + b;
    printf("Result: %d\n", result);

    // Suppress unused variable warnings
    (void)c;

    return 0;
}

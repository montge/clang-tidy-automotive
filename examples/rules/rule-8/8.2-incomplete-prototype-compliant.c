// MISRA C:2025 Rule 8.2 - Function prototypes shall be complete
// Category: Required
// Status: COMPLIANT
//
// This code complies with Rule 8.2 by using complete function
// prototypes with named parameters.

#include <stdio.h>

// Compliant: void means "no parameters"
int calculate(void);

// Compliant: Parameters are named
int add(int left, int right);

// Compliant: Modern function definition
int multiply(int left, int right) {
    return left * right;
}

int main(void) {
    int x = calculate();
    int y = add(1, 2);
    int z = multiply(3, 4);
    printf("Results: %d, %d, %d\n", x, y, z);
    return 0;
}

int calculate(void) {
    return 42;
}

int add(int left, int right) {
    return left + right;
}

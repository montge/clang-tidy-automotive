// MISRA C:2025 Rule 8.2 - Function prototypes shall be complete
// Category: Required
// Status: VIOLATION
//
// This code violates Rule 8.2 because function declarations do not
// have complete prototypes with named parameters.
//
// Expected diagnostic:
//   warning: function declaration has incomplete prototype
//   [automotive-uncomplete-function-prototype]

#include <stdio.h>

// VIOLATION: Empty parameter list means "any parameters" in C
int calculate();

// VIOLATION: Parameters not named
int add(int, int);

// VIOLATION: Old-style K&R function definition
int multiply(a, b)
    int a;
    int b;
{
    return a * b;
}

int main(void) {
    int x = calculate();
    int y = add(1, 2);
    int z = multiply(3, 4);
    printf("Results: %d, %d, %d\n", x, y, z);
    return 0;
}

int calculate() {
    return 42;
}

int add(int a, int b) {
    return a + b;
}

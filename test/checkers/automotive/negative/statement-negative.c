// Negative test file for: automotive statement checks
// Related MISRA C:2025 Rules: 15.1, 15.6, 15.7, 16.x
//
// This file contains code that should NOT trigger any warnings.
// All code here is compliant with MISRA rules.

// RUN: clang-tidy -checks='-*,automotive-avoid-goto,automotive-missing-compound,automotive-missing-else,automotive-missing-default-in-switch,automotive-missing-break-in-case' %s -- 2>&1 | FileCheck %s -allow-empty -check-prefix=CHECK-NEGATIVE
// CHECK-NEGATIVE-NOT: warning:

#include <stdbool.h>

//===----------------------------------------------------------------------===//
// Negative: No goto statements
//===----------------------------------------------------------------------===//

int process_with_return(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] < 0) {
            return -1;  // Using return instead of goto
        }
    }
    return 0;
}

int process_with_break(int *arr, int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] < 0) {
            result = -1;
            break;  // Using break instead of goto
        }
    }
    return result;
}

//===----------------------------------------------------------------------===//
// Negative: All compound statements have braces
//===----------------------------------------------------------------------===//

void test_if_with_braces(int x) {
    if (x > 0) {
        x++;
    }

    if (x > 10) {
        x = 10;
    } else {
        x = 0;
    }

    if (x == 0) {
        x = 1;
    } else if (x == 1) {
        x = 2;
    } else {
        x = 3;
    }
}

void test_loops_with_braces(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }

    int j = 0;
    while (j < size) {
        arr[j]++;
        j++;
    }

    int k = 0;
    do {
        arr[k]++;
        k++;
    } while (k < size);
}

//===----------------------------------------------------------------------===//
// Negative: All if-else-if chains end with else
//===----------------------------------------------------------------------===//

int classify_value(int x) {
    if (x < 0) {
        return -1;
    } else if (x == 0) {
        return 0;
    } else {
        return 1;
    }
}

void handle_status(int status) {
    if (status == 1) {
        // Handle status 1
    } else if (status == 2) {
        // Handle status 2
    } else if (status == 3) {
        // Handle status 3
    } else {
        // Handle all other status values
    }
}

//===----------------------------------------------------------------------===//
// Negative: All switch statements have default and break
//===----------------------------------------------------------------------===//

int handle_command(int cmd) {
    int result;
    switch (cmd) {
        case 1:
            result = 10;
            break;
        case 2:
            result = 20;
            break;
        case 3:
            result = 30;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

const char *get_status_name(int status) {
    switch (status) {
        case 0:
            return "IDLE";
        case 1:
            return "RUNNING";
        case 2:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

//===----------------------------------------------------------------------===//
// Negative: Boolean expressions in switch are avoided
//===----------------------------------------------------------------------===//

void handle_by_range(int value) {
    // Using if-else instead of switch on boolean
    if (value < 0) {
        // Handle negative
    } else if (value == 0) {
        // Handle zero
    } else {
        // Handle positive
    }
}

//===----------------------------------------------------------------------===//
// Negative: Labels are used
//===----------------------------------------------------------------------===//

// No labels defined since goto is not used
// This ensures no unused labels exist

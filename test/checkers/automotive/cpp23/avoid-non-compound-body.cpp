// RUN: %check_clang_tidy %s automotive-cpp23-req-9.3 %t
// Test for automotive-cpp23-req-9.3: Compound statement body
// Related MISRA C++:2023 Rule: 9.3

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_if_violations(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: if statement body shall be a compound statement [automotive-cpp23-req-9.3]
    if (x > 0)
        x++;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: if statement body shall be a compound statement [automotive-cpp23-req-9.3]
    if (x > 0)
        x++;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: else statement body shall be a compound statement [automotive-cpp23-req-9.3]
    else
        x--;
}

void test_loop_violations(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: while statement body shall be a compound statement [automotive-cpp23-req-9.3]
    while (x > 0)
        x--;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: for statement body shall be a compound statement [automotive-cpp23-req-9.3]
    for (int i = 0; i < 10; i++)
        x++;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: do statement body shall be a compound statement [automotive-cpp23-req-9.3]
    do
        x++;
    while (x < 100);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(int x) {
    if (x > 0) {
        x++;
    }

    if (x > 0) {
        x++;
    } else {
        x--;
    }

    while (x > 0) {
        x--;
    }

    for (int i = 0; i < 10; i++) {
        x++;
    }

    do {
        x++;
    } while (x < 100);

    // Switch with compound statement in each case
    switch (x) {
        case 1: {
            x++;
            break;
        }
        default: {
            x--;
            break;
        }
    }
}

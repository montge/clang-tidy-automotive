// RUN: %check_clang_tidy %s automotive-c23-req-14.3 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-req-14.3 %t -- -- -std=c11

// MISRA C:2025 Rule 14.3 (Required)
// Controlling expressions shall not be invariant.

void f(int x) {
    if (2 > 3) {
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: avoid controlling expression to always be evaluated to 'false' [automotive-c23-req-14.3]
    }

    while (1) { // Flagged - infinite loops are flagged
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: avoid controlling expression to always be evaluated to 'true' [automotive-c23-req-14.3]
        break;
    }

    do { } while (0); // Flagged - do-while(0) is flagged
    // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: avoid controlling expression to always be evaluated to 'false' [automotive-c23-req-14.3]

    for (int i = 0; i < 10; i++) { // OK - i changes
    }

    if (10 > 5) {
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: avoid controlling expression to always be evaluated to 'true' [automotive-c23-req-14.3]
    }
}

// RUN: %check_clang_tidy %s automotive-avoid-signal-header %t

// Test: signal.h shall not be used (MISRA Rule 21.5)

#include <signal.h>

// Violation - using signal function
void handler(int sig) {
    (void)sig;
}

void test_signal(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: Avoid function 'signal' from <signal.h> [automotive-avoid-signal-header]
    signal(SIGINT, handler);
}

// Use function to avoid unused warnings
void use_functions(void) {
    test_signal();
}

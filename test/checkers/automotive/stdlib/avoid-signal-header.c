// RUN: %check_clang_tidy %s automotive-c23-req-21.5 %t

// Test: signal.h shall not be used (MISRA Rule 21.5)

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: use of <signal.h> is not permitted
#include <signal.h>

// Violation - using signal function
void handler(int sig) {
    (void)sig;
}

void test_signal(void) {
    signal(SIGINT, handler);
}

// Use function to avoid unused warnings
void use_functions(void) {
    test_signal();
}

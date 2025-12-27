// RUN: %check_clang_tidy %s automotive-cpp23-req-21.10.3 %t
// Test for automotive-cpp23-req-21.10.3: <csignal> header shall not be used

#include <signal.h>

void handler(int sig) {
  (void)sig;
}

void test_signal() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: Avoid function 'signal' from <signal.h>
  signal(SIGINT, handler);
}

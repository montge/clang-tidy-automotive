// RUN: %check_clang_tidy %s automotive-cpp23-req-10.2 %t

// MISRA C++:2023 Rule 10.2 - At-exit handlers shall terminate by returning

// Mock declarations (no standard headers needed)
extern "C" {
  void exit(int);
  void abort(void);
  int atexit(void (*)(void));
  int at_quick_exit(void (*)(void));
}

namespace std {
  class runtime_error {
  public:
    runtime_error(const char*) {}
  };
}

void handler_throws() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: at-exit handler shall not throw exceptions; must terminate by returning (MISRA C++:2023 Rule 10.2)
  throw std::runtime_error("error");
}

void handler_exits() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: at-exit handler shall not call exit; must terminate by returning (MISRA C++:2023 Rule 10.2)
  exit(1);
}

void handler_aborts() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: at-exit handler shall not call abort; must terminate by returning (MISRA C++:2023 Rule 10.2)
  abort();
}

void compliant_handler() {
  // Perform cleanup
  return;  // Compliant - normal return
}

void register_handlers() {
  atexit(handler_throws);
  atexit(handler_exits);
  atexit(handler_aborts);
  atexit(compliant_handler);

  // Using at_quick_exit
  at_quick_exit(handler_throws);
  at_quick_exit(compliant_handler);
}

// Another compliant handler
void cleanup() {
  // Do cleanup work
  int x = 0;
  x++;
  // Implicit return - compliant
}

void test() {
  atexit(cleanup);
  atexit(&compliant_handler);
}

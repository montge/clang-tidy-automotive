// XFAIL: *
// RUN: %check_clang_tidy %s automotive-cpp23-req-10.1 %t

// MISRA C++:2023 Rule 10.1 - Memory shall not be allocated dynamically during static initialization

// Mock declarations
extern "C" {
  void* malloc(unsigned long);
  void* calloc(unsigned long, unsigned long);
  void* realloc(void*, unsigned long);
  void free(void*);
}

// CHECK-MESSAGES: :[[@LINE+1]]:19: warning: dynamic memory allocation (new) shall not occur during static initialization (MISRA C++:2023 Rule 10.1)
int* global_ptr = new int(42);

// CHECK-MESSAGES: :[[@LINE+1]]:21: warning: dynamic memory allocation (new) shall not occur during static initialization (MISRA C++:2023 Rule 10.1)
int* global_array = new int[10];

// Compliant - allocation not during static initialization
void function() {
  int* local_ptr = new int(42);  // OK - local allocation
  delete local_ptr;
}

class MyClass {
public:
  static int* static_ptr;
};

// Initialization in implementation file
int* MyClass::static_ptr = nullptr;  // Compliant - no allocation

void init() {
  // Compliant - allocation in function, not during static init
  MyClass::static_ptr = new int(100);
}

// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: dynamic memory allocation (malloc/calloc/realloc) shall not occur during static initialization (MISRA C++:2023 Rule 10.1)
void* global_malloc = malloc(100);

// Compliant
void allocateMemory() {
  void* local_malloc = malloc(100);  // OK
  free(local_malloc);
}

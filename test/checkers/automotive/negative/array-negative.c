// RUN: clang-tidy %s --checks='-*,automotive-avoid-variable-length-array,automotive-avoid-flexible-array-member,automotive-avoid-partial-array-init,automotive-c23-req-9.4,automotive-c23-adv-18.4' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger array warnings.

// Fixed-size arrays are allowed
int fixed_array[10];
int multi_array[2][3] = {{1, 2, 3}, {4, 5, 6}};

// Properly initialized arrays are allowed
int init_array[5] = {1, 2, 3, 4, 5};
int designated_full[5] = {[0] = 1, [1] = 2, [2] = 3, [3] = 4, [4] = 5};

// Static arrays in parameters (compliant usage)
void process_data(int data[static 10]) {
  (void)data;
}

// Struct without flexible array member
struct valid_struct {
  int x;
  int y;
  int data[10];
};

// Pointer usage without arithmetic (compliant)
void use_pointer(int *p) {
  int val = *p;
  (void)val;
}

// Array access via subscript operator (allowed even in MISRA)
void array_access(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = i;
  }
}

// CHECK-NOT: warning:

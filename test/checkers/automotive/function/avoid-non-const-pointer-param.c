// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-adv-8.13 %t

// MISRA C:2025 Rule 8.13: Pointer parameters should be const if not modified

// CHECK-MESSAGES: :[[@LINE+1]]:28: warning: pointer parameter 'p' should be declared const as it is not modified [automotive-c25-adv-8.13]
void read_only_pointer(int *p) {
  int x = *p;  // Only reading through pointer
}

// Compliant - already const
void const_pointer(const int *p) {
  int x = *p;  // OK - already const
}

// CHECK-MESSAGES: :[[@LINE+1]]:35: warning: pointer parameter 'data' should be declared const as it is not modified [automotive-c25-adv-8.13]
int sum_array(int *data, int size) {
  int total = 0;
  for (int i = 0; i < size; i++) {
    total += data[i];  // Only reading
  }
  return total;
}

// Compliant - const array parameter
int sum_array_const(const int *data, int size) {
  int total = 0;
  for (int i = 0; i < size; i++) {
    total += data[i];
  }
  return total;
}

// Non-compliant - modifies through pointer
void modify_pointer(int *p) {
  *p = 42;  // OK - modifies the value, no warning expected
}

// Non-compliant - increments pointer
void increment_value(int *p) {
  (*p)++;   // OK - modifies the value, no warning expected
}

// CHECK-MESSAGES: :[[@LINE+1]]:32: warning: pointer parameter 's' should be declared const as it is not modified [automotive-c25-adv-8.13]
int string_length(char *s) {
  int len = 0;
  while (*s++) {
    len++;
  }
  return len;
}

// Compliant - const string parameter
int string_length_const(const char *s) {
  int len = 0;
  while (*s++) {
    len++;
  }
  return len;
}

struct Point {
  int x;
  int y;
};

// CHECK-MESSAGES: :[[@LINE+1]]:36: warning: pointer parameter 'pt' should be declared const as it is not modified [automotive-c25-adv-8.13]
int get_x_coord(struct Point *pt) {
  return pt->x;  // Only reading
}

// Compliant - const struct pointer
int get_x_coord_const(const struct Point *pt) {
  return pt->x;
}

// Non-compliant - modifies struct member
void set_x_coord(struct Point *pt, int x) {
  pt->x = x;  // OK - modifies the struct, no warning expected
}

// Compliant - void pointer (should be skipped)
void process_data(void *data, int size) {
  // Generic interface - should not warn
  char *bytes = (char *)data;
}

// CHECK-MESSAGES: :[[@LINE+1]]:32: warning: pointer parameter 'arr' should be declared const as it is not modified [automotive-c25-adv-8.13]
int find_max(int *arr, int n) {
  int max = arr[0];
  for (int i = 1; i < n; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

// Compliant - passed to function expecting non-const
void helper(int *p);

void caller(int *p) {
  helper(p);  // OK - passed to non-const function, no warning expected
}

// Function pointer - should be skipped
typedef void (*callback_t)(int);

void register_callback(callback_t cb) {
  // Function pointer - should not warn
  cb(42);
}

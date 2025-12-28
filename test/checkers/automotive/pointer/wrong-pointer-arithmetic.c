// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-mand-18.10 %t

void test_pointer_arithmetic(void) {
  int x;
  int *p = &x;

  // Violations: pointer arithmetic on non-array pointers
  p++;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: pointer arithmetic shall only be applied to pointers that address array elements

  p--;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: pointer arithmetic shall only be applied to pointers that address array elements

  int *q = p + 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: pointer arithmetic shall only be applied to pointers that address array elements

  p += 2;
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: pointer arithmetic shall only be applied to pointers that address array elements

  // Compliant: array element pointer arithmetic
  int arr[10];
  int *arr_ptr = arr;
  arr_ptr++;  // OK: array pointer
  arr_ptr--;  // OK: array pointer

  int *arr_ptr2 = arr + 1;  // OK: array pointer
  arr_ptr2 += 3;  // OK: array pointer

  // Compliant: direct array access
  arr[5] = 10;  // OK
  int val = arr[3];  // OK

  // Compliant: pointer from array subscript
  int *arr_elem = &arr[2];
  arr_elem++;  // OK: points to array element
}

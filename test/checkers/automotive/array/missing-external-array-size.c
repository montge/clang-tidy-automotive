// RUN: %check_clang_tidy %s automotive-missing-external-array-size %t

// Test: External arrays should have explicit size (MISRA Rule 8.11)

// Violations - external arrays without explicit size
extern int arr1[];
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: missing explicit size of external array 'arr1'

extern char str[];
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: missing explicit size of external array 'str'

extern double data[];
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: missing explicit size of external array 'data'

// Compliant - external arrays with explicit size
extern int arr2[10];
extern char str2[256];
extern double data2[100];

// Compliant - non-external arrays without size (defined with initializer)
int local_arr[] = {1, 2, 3};
static int static_arr[] = {4, 5, 6};

// Compliant - external arrays defined with size
int defined_arr[5] = {1, 2, 3, 4, 5};

// Compliant - pointers (not arrays)
extern int *ptr;
extern char *str_ptr;

// Test function to use the arrays (avoid unused warnings)
void use_arrays(void) {
    (void)arr2[0];
    (void)str2[0];
    (void)data2[0];
    (void)local_arr[0];
    (void)static_arr[0];
    (void)defined_arr[0];
    (void)ptr;
    (void)str_ptr;
}

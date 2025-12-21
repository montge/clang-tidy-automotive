// RUN: %check_clang_tidy %s automotive-c23-req-21.9 %t
// Test for automotive-c23-req-21.9
// Related MISRA C:2025 Rule: 21.9

#include <stdlib.h>

int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_violations(void) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int key = 5;
    size_t n = sizeof(arr) / sizeof(arr[0]);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: qsort shall not be used [automotive-c23-req-21.9]
    qsort(arr, n, sizeof(int), compare_ints);

    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: bsearch shall not be used [automotive-c23-req-21.9]
    int *result = bsearch(&key, arr, n, sizeof(int), compare_ints);
    (void)result;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Compliant: Use a specific sorting algorithm with known behavior
void insertion_sort(int *arr, size_t n) {
    for (size_t i = 1; i < n; i++) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j-1] > key) {
            arr[j] = arr[j-1];
            j--;
        }
        arr[j] = key;
    }
}

// Compliant: Use a specific search algorithm with known behavior
int *binary_search(const int *arr, size_t n, int key) {
    size_t low = 0, high = n;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (arr[mid] < key) {
            low = mid + 1;
        } else if (arr[mid] > key) {
            high = mid;
        } else {
            return (int*)&arr[mid];
        }
    }
    return NULL;
}

void test_compliant(void) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    insertion_sort(arr, n);

    int key = 5;
    int *result = binary_search(arr, n, key);
    (void)result;
}

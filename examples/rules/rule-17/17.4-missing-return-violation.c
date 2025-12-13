// MISRA C:2025 Rule 17.4 - All exit paths shall have explicit return
// Category: Mandatory
// Status: VIOLATION
//
// This code violates Rule 17.4 because the non-void function does not
// have a return statement on all paths. This leads to undefined behavior.
//
// Expected diagnostic:
//   warning: non-void function does not return a value on all paths
//   [automotive-missing-return-in-non-void-function]

int get_sign(int value) {
    if (value > 0) {
        return 1;
    } else if (value < 0) {
        return -1;
    }
    // VIOLATION: Missing return for value == 0 case
}

int find_max(int* arr, int size) {
    if (size <= 0) {
        return 0;
    }

    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    // VIOLATION: Missing return statement
}

int main(void) {
    int sign = get_sign(0);
    int arr[] = {1, 5, 3, 9, 2};
    int max = find_max(arr, 5);
    return sign + max;  // Undefined behavior
}

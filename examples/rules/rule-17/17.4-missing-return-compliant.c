// MISRA C:2025 Rule 17.4 - All exit paths shall have explicit return
// Category: Mandatory
// Status: COMPLIANT
//
// This code complies with Rule 17.4 by ensuring all code paths in
// non-void functions have explicit return statements.

int get_sign(int value) {
    int result;

    if (value > 0) {
        result = 1;
    } else if (value < 0) {
        result = -1;
    } else {
        result = 0;  // Handle value == 0
    }

    return result;  // Single exit point
}

int find_max(int* arr, int size) {
    int max;

    if (size <= 0) {
        max = 0;
    } else {
        max = arr[0];
        for (int i = 1; i < size; i++) {
            if (arr[i] > max) {
                max = arr[i];
            }
        }
    }

    return max;  // Single exit point
}

int main(void) {
    int sign = get_sign(0);
    int arr[] = {1, 5, 3, 9, 2};
    int max = find_max(arr, 5);
    return sign + max;
}

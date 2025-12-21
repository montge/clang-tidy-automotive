// RUN: %check_clang_tidy %s automotive-c23-req-9.5 %t -- -- -std=gnu11

// Test: Designated initializer bounds checking (MISRA Rule 9.5)

// Compliant - valid designated initializers
int arr1[5] = {[0] = 1, [1] = 2, [4] = 5};
int arr2[3] = {[2] = 100};
int arr3[10] = {[0] = 0, [5] = 50, [9] = 90};

// Compliant - struct designated initializers
struct Point {
    int x;
    int y;
};

struct Point p1 = {.x = 10, .y = 20};

// Compliant - nested designated initializers
struct Line {
    struct Point start;
    struct Point end;
};

struct Line line = {.start.x = 0, .start.y = 0, .end.x = 100, .end.y = 100};

// GNU extension - array range designators (compliant)
int range_arr[10] = {[0 ... 4] = 1, [5 ... 9] = 2};

// GNU extension - array range with invalid range (start > end)
// Note: This causes a compiler error and cannot be tested
// int bad_range[10] = {[5 ... 2] = 1};

// Test negative indices (if compiler allows)
// Note: Most compilers reject negative indices at compile time
// but the check should catch them if they make it through

// Compliant - using computed constants
#define START_INDEX 2
#define END_INDEX 7
int const_arr[10] = {[START_INDEX] = 10, [END_INDEX] = 70};

// Test array of structs with designated initializers
struct Point points[3] = {
    [0] = {.x = 0, .y = 0},
    [1] = {.x = 1, .y = 1},
    [2] = {.x = 2, .y = 2}
};

// Test 2D array designated initializers
int matrix[3][3] = {
    [0] = {1, 0, 0},
    [1] = {0, 1, 0},
    [2] = {0, 0, 1}
};

// Function to use arrays (avoid unused warnings)
void use_arrays(void) {
    (void)arr1[0];
    (void)arr2[0];
    (void)arr3[0];
    (void)p1.x;
    (void)line.start.x;
    (void)range_arr[0];
    (void)const_arr[0];
    (void)points[0].x;
    (void)matrix[0][0];
}

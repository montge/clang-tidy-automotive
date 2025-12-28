// RUN: %check_clang_tidy %s automotive-cpp23-req-13.1 %t

// MISRA C++:2023 Rule 13.1 - An object shall not be accessed out of bounds

void testArrayAccess() {
  int arr[5] = {0, 1, 2, 3, 4};

  // Compliant - valid indices
  int x = arr[0];
  int y = arr[4];

  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: array subscript is out of bounds; valid range is [0, 4] (MISRA C++:2023 Rule 13.1)
  int z = arr[5];

  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: array subscript is out of bounds; valid range is [0, 4] (MISRA C++:2023 Rule 13.1)
  int w = arr[10];

  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: array subscript is negative; object accessed out of bounds (MISRA C++:2023 Rule 13.1)
  int n = arr[-1];
}

void testConstantIndexAccess() {
  constexpr int SIZE = 3;
  int buffer[SIZE];

  // Compliant
  buffer[0] = 1;
  buffer[1] = 2;
  buffer[2] = 3;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: array subscript is out of bounds; valid range is [0, 2] (MISRA C++:2023 Rule 13.1)
  buffer[3] = 4;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: array subscript is out of bounds; valid range is [0, 2] (MISRA C++:2023 Rule 13.1)
  buffer[SIZE + 2] = 5;
}

void testMultiDimensionalArray() {
  int matrix[2][3];

  // Compliant
  matrix[0][0] = 1;
  matrix[1][2] = 2;

  // Non-compliant - out of bounds on second dimension
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: array subscript is out of bounds; valid range is [0, 2] (MISRA C++:2023 Rule 13.1)
  matrix[0][3] = 3;

  // Note: First dimension out of bounds
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: array subscript is out of bounds; valid range is [0, 1] (MISRA C++:2023 Rule 13.1)
  matrix[2][0] = 4;
}

void testCharArray() {
  char str[10];

  // Compliant
  str[0] = 'H';
  str[9] = '\0';

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: array subscript is out of bounds; valid range is [0, 9] (MISRA C++:2023 Rule 13.1)
  str[10] = 'X';
}

void testVariableIndexNotDetected(int idx) {
  int arr[5];

  // Runtime index - cannot be detected statically
  int x = arr[idx];  // No warning - runtime check needed
}

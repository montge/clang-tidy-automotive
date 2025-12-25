// RUN: %check_clang_tidy %s automotive-cpp23-req-13.1 %t
// Test for automotive-cpp23-req-13.1: Copy/move operations should not have
// overlapping source and destination

#include <cstring>
#include <algorithm>

class MyClass {
public:
  int value;

  MyClass& operator=(const MyClass& other) {
    if (this != &other) {
      value = other.value;
    }
    return *this;
  }

  MyClass& operator=(MyClass&& other) noexcept {
    if (this != &other) {
      value = other.value;
    }
    return *this;
  }
};

void test_self_assignment() {
  MyClass obj;
  obj.value = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  obj = obj;

  MyClass obj2;
  obj2.value = 10;

  // OK: different objects
  obj = obj2;
}

void test_self_assignment_builtin() {
  int x = 5;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  x = x;

  int y = 10;
  x = y;  // OK: different variables
}

void test_array_self_assignment() {
  int arr[10];
  arr[0] = 1;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  arr[0] = arr[0];

  arr[1] = arr[0];  // OK: different elements
}

void test_pointer_self_assignment() {
  int value = 42;
  int* ptr = &value;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  *ptr = *ptr;

  int other = 10;
  int* ptr2 = &other;
  *ptr = *ptr2;  // OK: different pointers
}

void test_member_self_assignment() {
  MyClass obj;
  obj.value = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  obj.value = obj.value;

  MyClass obj2;
  obj.value = obj2.value;  // OK: different objects
}

void test_memcpy_same_buffer() {
  char buffer[100];

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: memcpy called with potentially overlapping source and destination; use memmove for overlapping ranges [automotive-cpp23-req-13.1]
  std::memcpy(buffer, buffer, 50);

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: memcpy called with potentially overlapping source and destination; use memmove for overlapping ranges [automotive-cpp23-req-13.1]
  std::memcpy(buffer + 5, buffer, 20);

  char buffer2[100];
  std::memcpy(buffer, buffer2, 50);  // OK: different buffers

  // OK: memmove handles overlapping ranges
  std::memmove(buffer, buffer, 50);
  std::memmove(buffer + 5, buffer, 20);
}

void test_std_copy_overlap() {
  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: std::copy called with overlapping source and destination ranges; behavior is undefined [automotive-cpp23-req-13.1]
  std::copy(arr, arr + 5, arr);

  int arr2[10];
  std::copy(arr, arr + 5, arr2);  // OK: different arrays
}

struct Point {
  int x, y;
};

void test_struct_member_assignment() {
  Point p1{1, 2};

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  p1.x = p1.x;

  Point p2{3, 4};
  p1.x = p2.x;  // OK: different points
}

void test_complex_expressions() {
  int arr[5] = {1, 2, 3, 4, 5};
  int i = 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  arr[i] = arr[i];

  arr[i] = arr[i + 1];  // OK: different indices
}

class WithCopyConstructor {
public:
  int data;

  WithCopyConstructor(const WithCopyConstructor& other) : data(other.data) {}
};

void test_move_semantics() {
  MyClass obj;
  obj.value = 100;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  obj = static_cast<MyClass&&>(obj);

  MyClass obj2;
  obj = static_cast<MyClass&&>(obj2);  // OK: different objects
}

void test_reference_self_assignment() {
  int x = 5;
  int& ref = x;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  x = ref;

  int y = 10;
  int& ref2 = y;
  x = ref2;  // OK: different variables
}

void test_trivial_self_assignment() {
  // Trivial types where self-assignment is harmless (implementation may skip warning)
  int simple = 5;
  simple = simple;  // May or may not warn depending on implementation
}

void compliant_examples() {
  // All of these are compliant - no overlapping copy operations

  MyClass obj1, obj2;
  obj1 = obj2;  // Different objects

  int arr1[10], arr2[10];
  std::copy(arr1, arr1 + 10, arr2);  // Different arrays

  char buf1[100], buf2[100];
  std::memcpy(buf1, buf2, 100);  // Different buffers

  // Using memmove for potentially overlapping ranges
  char buf[100];
  std::memmove(buf + 10, buf, 50);

  int x = 5, y = 10;
  x = y;  // Different variables

  Point p1{1, 2}, p2{3, 4};
  p1 = p2;  // Different structs
  p1.x = p2.x;  // Different struct members
}

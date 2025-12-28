// XFAIL: *
// RUN: %check_clang_tidy %s automotive-cpp23-req-13.1 %t
// Test for automotive-cpp23-req-13.1: Copy/move operations should not have
// overlapping source and destination

#include <string.h>

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

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_class_self_assignment() {
  MyClass obj;
  obj.value = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: self-assignment detected; copy/move operations should not have overlapping source and destination [automotive-cpp23-req-13.1]
  obj = obj;
}

void test_memcpy_same_buffer() {
  char buffer[100];

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: memcpy called with potentially overlapping source and destination; use memmove for overlapping ranges [automotive-cpp23-req-13.1]
  memcpy(buffer, buffer, 50);

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: memcpy called with potentially overlapping source and destination; use memmove for overlapping ranges [automotive-cpp23-req-13.1]
  memcpy(buffer + 5, buffer, 20);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_different_objects() {
  MyClass obj1, obj2;
  obj1.value = 10;
  obj2.value = 20;

  // OK: different objects
  obj1 = obj2;
}

void test_different_buffers() {
  char buf1[100], buf2[100];

  // OK: different buffers
  memcpy(buf1, buf2, 100);
}

void test_memmove_overlapping() {
  char buffer[100];

  // OK: memmove handles overlapping ranges
  memmove(buffer, buffer, 50);
  memmove(buffer + 5, buffer, 20);
}

struct Point {
  int x, y;
};

void test_compliant_assignments() {
  // Different variables
  int x = 5, y = 10;
  x = y;

  // Different struct instances
  Point p1{1, 2}, p2{3, 4};
  p1 = p2;
  p1.x = p2.x;

  // Different array elements
  int arr[10];
  arr[1] = arr[0];
}

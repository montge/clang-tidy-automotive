// RUN: clang-tidy %s --checks='-*,automotive-avoid-undef,automotive-c23-req-20.3,automotive-avoid-macro-named-as-ckeyword,automotive-c23-req-5.4' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger preprocessor warnings.

// Proper header inclusion
#include <stddef.h>
#include <stdint.h>

// Proper macro definitions
#define MAX_SIZE 100
#define MIN_VALUE 0
#define BUFFER_LEN (MAX_SIZE * 2)

// Function-like macros with proper names
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Conditional compilation
#ifndef MY_HEADER_H
#define MY_HEADER_H

#ifdef DEBUG
#define LOG_ENABLED 1
#else
#define LOG_ENABLED 0
#endif

#endif

// Using macros properly
void use_macros(void) {
  int arr[MAX_SIZE];
  int x = SQUARE(5);
  int y = MAX(10, 20);
  (void)arr;
  (void)x;
  (void)y;
}

// CHECK-NOT: warning:

// RUN: %check_clang_tidy %s automotive-c25-req-9.6 %t

// MISRA C:2025 Rule 9.6: An initializer using chained designators shall not
// contain initializers without designators.

struct Inner {
  int x;
  int y;
};

struct Outer {
  int a;
  struct Inner inner;
  int b;
};

// Non-compliant: chained designator mixed with non-designated initializer
struct Outer bad1 = {
  .inner.x = 1,  // Chained designator
  2              // Non-designated - violation
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: initializer list with chained designators contains non-designated initializer; all initializers should use designators
};

// Compliant: all designated
struct Outer good1 = {
  .a = 0,
  .inner.x = 1,  // Chained designator
  .inner.y = 2,  // Chained designator
  .b = 3
};

// Compliant: no chained designators, mixing is fine
struct Outer good2 = {
  .a = 0,
  .inner = { 1, 2 },  // Not chained - nested init list
  3                   // Non-designated but no chained designators
};

// Compliant: all positional (no designators at all)
struct Outer good3 = { 0, { 1, 2 }, 3 };

// Compliant: simple designators without chaining
struct Inner good4 = {
  .x = 1,
  .y = 2
};

// Non-compliant: another example
struct Outer bad2 = {
  1,               // Non-designated
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: initializer list with chained designators contains non-designated initializer; all initializers should use designators
  .inner.x = 2,    // Chained designator
  .b = 3
};

// Deeply nested structure
struct Deep {
  struct Outer outer;
  int c;
};

// Non-compliant: deeply chained designator with non-designated
struct Deep bad3 = {
  .outer.inner.x = 1,  // Chained designator (3 levels)
  2                    // Non-designated - violation
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: initializer list with chained designators contains non-designated initializer; all initializers should use designators
};

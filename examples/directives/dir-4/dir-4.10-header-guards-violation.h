// MISRA C:2025 Directive 4.10 - Header guards
// Category: Required
// Status: VIOLATION
//
// Header files should use precautions to prevent multiple inclusion.
// This header has no include guard.

// VIOLATION: No include guard or #pragma once
// Multiple inclusion will cause redefinition errors

typedef struct {
    int x;
    int y;
} Point;

extern int global_counter;

void initialize_point(Point *p);
int get_distance(const Point *p1, const Point *p2);

// If this header is included multiple times:
// - typedef Point will be redefined (error)
// - Function declarations will be repeated (usually ok but wasteful)
// - extern declaration repeated (usually ok but wasteful)

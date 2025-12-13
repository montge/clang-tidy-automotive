// MISRA C:2025 Directive 4.10 - Header guards
// Category: Required
// Status: COMPLIANT
//
// Header files use include guards to prevent multiple inclusion.

#ifndef POINT_H
#define POINT_H

// COMPLIANT: Traditional include guard
// Guard macro name should be unique (based on file path/name)

typedef struct {
    int x;
    int y;
} Point;

extern int global_counter;

void initialize_point(Point *p);
int get_distance(const Point *p1, const Point *p2);

#endif /* POINT_H */

// Alternative COMPLIANT approach using #pragma once:
// Note: #pragma once is widely supported but not standard C
//
// #pragma once
// ... header contents ...
//
// For maximum portability, prefer #ifndef/#define/#endif guards.

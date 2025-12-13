// MISRA C:2025 Rule 2.3 - Unused type declarations
// Category: Advisory
// Status: COMPLIANT
//
// All type declarations in the project are used.

#include <stdint.h>

// COMPLIANT: All types are used

typedef struct {
    int x;
    int y;
} Point;

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR
} State;

// Using Point type
Point create_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

// Using State type
State get_current_state(void) {
    return STATE_IDLE;
}

void process_state(State s) {
    switch (s) {
        case STATE_IDLE:
            // Handle idle
            break;
        case STATE_RUNNING:
            // Handle running
            break;
        case STATE_ERROR:
            // Handle error
            break;
        default:
            // Handle unknown
            break;
    }
}

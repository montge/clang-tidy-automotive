// MISRA C:2025 Rule 16.4 - Every switch statement shall have a default label
// Category: Required
// Status: COMPLIANT
//
// This code complies with Rule 16.4 by including a default case
// that handles any unexpected values appropriately.

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_STOPPED
} State;

const char* get_state_name(State state) {
    const char* name;

    switch (state) {
        case STATE_IDLE:
            name = "Idle";
            break;
        case STATE_RUNNING:
            name = "Running";
            break;
        case STATE_STOPPED:
            name = "Stopped";
            break;
        default:
            // Handle unexpected values
            name = "Unknown";
            break;
    }

    return name;
}

int main(void) {
    State s = STATE_RUNNING;
    const char* name = get_state_name(s);
    return 0;
}

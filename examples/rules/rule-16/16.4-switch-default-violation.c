// MISRA C:2025 Rule 16.4 - Every switch statement shall have a default label
// Category: Required
// Status: VIOLATION
//
// This code violates Rule 16.4 because the switch statement does not
// have a default case, which means unexpected values are silently ignored.
//
// Expected diagnostic:
//   warning: switch statement missing default case
//   [automotive-missing-default-in-switch-stmt]

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_STOPPED
} State;

const char* get_state_name(State state) {
    const char* name;

    switch (state) {  // VIOLATION: no default case
        case STATE_IDLE:
            name = "Idle";
            break;
        case STATE_RUNNING:
            name = "Running";
            break;
        case STATE_STOPPED:
            name = "Stopped";
            break;
    }

    return name;  // Undefined if state is invalid
}

int main(void) {
    State s = STATE_RUNNING;
    const char* name = get_state_name(s);
    return 0;
}

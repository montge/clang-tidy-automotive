// MISRA C:2025 Rule 20.10 - # and ## operators
// Category: Advisory
// Status: COMPLIANT
//
// # and ## operators are avoided; explicit code is used instead.

#include <stdint.h>

// COMPLIANT: Use explicit string literals instead of stringize
static const char *const error_names[] = {
    "ERROR_NONE",
    "ERROR_TIMEOUT",
    "ERROR_OVERFLOW",
    "ERROR_INVALID"
};

typedef enum {
    ERROR_NONE = 0,
    ERROR_TIMEOUT = 1,
    ERROR_OVERFLOW = 2,
    ERROR_INVALID = 3
} ErrorCode;

const char *get_error_name(ErrorCode code) {
    if (code <= ERROR_INVALID) {
        return error_names[code];
    }
    return "ERROR_UNKNOWN";
}

// COMPLIANT: Use explicit function names instead of token paste
void sensor_init(void);
void sensor_read(void);
void sensor_reset(void);

// COMPLIANT: Use arrays/structs instead of generated variables
typedef struct {
    int32_t value;
    const char *name;
} Variable;

static Variable variables[] = {
    {0, "count"},
    {0, "total"},
    {0, "average"}
};

// COMPLIANT: Use X-macros pattern only if necessary,
// but prefer explicit code for clarity

void example(void) {
    ErrorCode err = ERROR_TIMEOUT;
    const char *name = get_error_name(err);
    (void)name;

    variables[0].value = 42;
}

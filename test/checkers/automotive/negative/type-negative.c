// Negative test file for: automotive type checks
// Related MISRA C:2025 Rules: 6.1, 6.2, 8.1, 8.12, 19.2
//
// This file contains code that should NOT trigger any warnings.
// All code here is compliant with MISRA rules.

// RUN: %check_clang_tidy %s automotive-wrong-bitfield-type,automotive-avoid-signed-single-bitfield,automotive-implicit-int,automotive-unique-enum-value,automotive-avoid-union %t
// CHECK-MESSAGES-NOT: warning:

#include <stdbool.h>
#include <stdint.h>

//===----------------------------------------------------------------------===//
// Negative: Proper bitfield types
//===----------------------------------------------------------------------===//

struct ProperBitfields {
    unsigned int flags : 4;       // Unsigned int - OK
    signed int value : 8;         // Signed int with more than 1 bit - OK
    _Bool enabled : 1;            // Boolean single bit - OK
    unsigned char status : 3;     // Unsigned char - OK
    unsigned int : 0;             // Anonymous zero-width for alignment - OK
};

struct SingleBitCompliant {
    unsigned int flag1 : 1;  // Unsigned single bit - OK
    _Bool flag2 : 1;         // Boolean single bit - OK
};

//===----------------------------------------------------------------------===//
// Negative: Explicit types (no implicit int)
//===----------------------------------------------------------------------===//

int explicit_return(int x) {
    return x * 2;
}

void explicit_void(void) {
    return;
}

static const int CONST_VALUE = 100;
static int static_var = 0;
extern int extern_var;

//===----------------------------------------------------------------------===//
// Negative: Unique enum values
//===----------------------------------------------------------------------===//

typedef enum {
    STATE_IDLE = 0,
    STATE_INIT = 1,
    STATE_RUNNING = 2,
    STATE_PAUSED = 3,
    STATE_ERROR = 4,
    STATE_STOPPED = 5
} State;

typedef enum {
    COLOR_RED = 0xFF0000,
    COLOR_GREEN = 0x00FF00,
    COLOR_BLUE = 0x0000FF,
    COLOR_WHITE = 0xFFFFFF,
    COLOR_BLACK = 0x000000
} Color;

// Implicit values are unique
typedef enum {
    FIRST,   // 0
    SECOND,  // 1
    THIRD,   // 2
    FOURTH   // 3
} Ordinal;

//===----------------------------------------------------------------------===//
// Negative: No unions used
//===----------------------------------------------------------------------===//

// Using struct instead of union
struct DataVariant {
    int type;
    int32_t int_value;
    float float_value;
    char string_value[32];
};

// Using explicit byte arrays for type punning
struct FloatBytes {
    float value;
};

uint8_t get_float_byte(struct FloatBytes *fb, int index) {
    uint8_t bytes[sizeof(float)];
    __builtin_memcpy(bytes, &fb->value, sizeof(float));
    return bytes[index];
}

//===----------------------------------------------------------------------===//
// Negative: Proper struct definitions
//===----------------------------------------------------------------------===//

struct Point {
    int32_t x;
    int32_t y;
};

struct Rectangle {
    struct Point top_left;
    struct Point bottom_right;
};

typedef struct {
    uint8_t id;
    uint16_t length;
    uint8_t data[64];
} Message;

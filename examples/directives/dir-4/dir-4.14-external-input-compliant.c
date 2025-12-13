// MISRA C:2025 Directive 4.14 - External input validation
// Category: Required
// Status: COMPLIANT
//
// All external inputs are validated before use.
// Invalid inputs are rejected or clamped to safe values.

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// COMPLIANT: Validate sensor data against expected ranges
typedef struct {
    int16_t temperature;
    uint16_t pressure;
    uint8_t humidity;
} SensorData;

typedef enum {
    SENSOR_OK,
    SENSOR_TEMP_OUT_OF_RANGE,
    SENSOR_PRESSURE_OUT_OF_RANGE,
    SENSOR_HUMIDITY_OUT_OF_RANGE
} SensorStatus;

#define TEMP_MIN    (-40)
#define TEMP_MAX    (125)
#define PRESSURE_MIN (300U)
#define PRESSURE_MAX (1100U)
#define HUMIDITY_MAX (100U)

SensorStatus validate_sensor_reading(const SensorData *reading) {
    if (reading == NULL) {
        return SENSOR_TEMP_OUT_OF_RANGE;  // Invalid pointer
    }

    if ((reading->temperature < TEMP_MIN) || (reading->temperature > TEMP_MAX)) {
        return SENSOR_TEMP_OUT_OF_RANGE;
    }

    if ((reading->pressure < PRESSURE_MIN) || (reading->pressure > PRESSURE_MAX)) {
        return SENSOR_PRESSURE_OUT_OF_RANGE;
    }

    if (reading->humidity > HUMIDITY_MAX) {
        return SENSOR_HUMIDITY_OUT_OF_RANGE;
    }

    return SENSOR_OK;
}

bool process_sensor_reading(const SensorData *reading) {
    if (validate_sensor_reading(reading) != SENSOR_OK) {
        return false;  // Reject invalid data
    }

    int scaled_temp = reading->temperature * 10;
    int pressure_kpa = (int)reading->pressure / 10;
    (void)scaled_temp;
    (void)pressure_kpa;
    return true;
}

// COMPLIANT: Validate message length before processing
#define MAX_MESSAGE_DATA_SIZE 32U

typedef struct {
    uint8_t command;
    uint8_t data[MAX_MESSAGE_DATA_SIZE];
    uint8_t length;
} ExternalMessage;

bool handle_message(const ExternalMessage *msg) {
    if (msg == NULL) {
        return false;
    }

    // Validate length against actual buffer size
    if (msg->length > MAX_MESSAGE_DATA_SIZE) {
        return false;  // Invalid length
    }

    for (uint8_t i = 0U; i < msg->length; i++) {
        // Safe to access msg->data[i]
        (void)msg->data[i];
    }
    return true;
}

// COMPLIANT: Validate external index before use
#define LOOKUP_TABLE_SIZE 10U
static int lookup_table[LOOKUP_TABLE_SIZE];

bool get_value(uint8_t external_index, int *result) {
    if (result == NULL) {
        return false;
    }

    if (external_index >= LOOKUP_TABLE_SIZE) {
        return false;  // Index out of bounds
    }

    *result = lookup_table[external_index];
    return true;
}

// COMPLIANT: Validate configuration size before applying
bool apply_configuration(const char *config_data, size_t size) {
    if (config_data == NULL) {
        return false;
    }

    // Validate size matches expected configuration
    if (size != sizeof(lookup_table)) {
        return false;  // Invalid configuration size
    }

    (void)memcpy(lookup_table, config_data, size);
    return true;
}

// MISRA C:2025 Directive 4.14 - External input validation
// Category: Required
// Status: VIOLATION
//
// All external inputs should be validated before use.
// External inputs include: user input, file data, network data,
// sensor readings, inter-process communication.

#include <stdint.h>
#include <string.h>

// VIOLATION: No validation of external sensor data
typedef struct {
    int16_t temperature;  // Expected range: -40 to +125 Celsius
    uint16_t pressure;    // Expected range: 300 to 1100 hPa
    uint8_t humidity;     // Expected range: 0 to 100 %
} SensorData;

void process_sensor_reading(SensorData *reading) {
    // Directly using external data without validation
    // Could cause issues if values are out of range
    int scaled_temp = reading->temperature * 10;
    int pressure_kpa = reading->pressure / 10;
    (void)scaled_temp;
    (void)pressure_kpa;
}

// VIOLATION: No validation of message from external system
typedef struct {
    uint8_t command;
    uint8_t data[32];
    uint8_t length;
} ExternalMessage;

void handle_message(const ExternalMessage *msg) {
    // length could be > 32, causing buffer overread
    for (uint8_t i = 0; i < msg->length; i++) {
        // Process msg->data[i] - potential out-of-bounds
        (void)msg->data[i];
    }
}

// VIOLATION: No validation of index from external source
static int lookup_table[10];

int get_value(uint8_t external_index) {
    // external_index could be >= 10
    return lookup_table[external_index];
}

// VIOLATION: No validation of configuration from external file
void apply_configuration(const char *config_data, size_t size) {
    // Trusting external data to be well-formed
    // Could contain malicious or corrupted data
    (void)memcpy(lookup_table, config_data, size);
}

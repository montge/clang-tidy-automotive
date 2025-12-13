// MISRA C:2025 Rule 5.1 - External identifier uniqueness
// Category: Required
// Status: VIOLATION
//
// External identifiers shall be distinct within the first 31 characters.
// (C99 guarantees 31 significant characters for external identifiers)

// VIOLATION: These identifiers differ only after 31 characters
extern int very_long_external_identifier_name_for_configuration_setting_alpha;
extern int very_long_external_identifier_name_for_configuration_setting_beta;
// Both may be treated as the same identifier by the linker

// VIOLATION: Similar identifiers with trailing numbers
extern void process_sensor_data_from_module_1(void);
extern void process_sensor_data_from_module_2(void);
// Difference is at position 34, beyond guaranteed significance

// VIOLATION: Identifiers that could collide on limited linkers
extern int abcdefghijklmnopqrstuvwxyz12345_first;
extern int abcdefghijklmnopqrstuvwxyz12345_second;

// Note: Modern compilers typically support longer identifiers,
// but MISRA requires portability to the C standard minimum.

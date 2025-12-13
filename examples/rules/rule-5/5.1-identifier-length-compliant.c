// MISRA C:2025 Rule 5.1 - External identifier uniqueness
// Category: Required
// Status: COMPLIANT
//
// External identifiers are distinct within the first 31 characters.

// COMPLIANT: Identifiers differ within first 31 characters
extern int config_setting_alpha;
extern int config_setting_beta;

// COMPLIANT: Use prefixes that differ early
extern void sensor_mod1_process(void);
extern void sensor_mod2_process(void);

// COMPLIANT: Use abbreviations to keep names short
extern int cfg_alpha;  // configuration alpha
extern int cfg_beta;   // configuration beta

// COMPLIANT: Meaningful short names
extern void init_system(void);
extern void init_sensors(void);
extern void init_network(void);

// Best practice: Keep external identifiers under 31 characters
// and make them differ as early as possible in the name.

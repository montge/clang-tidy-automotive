// MISRA C:2025 Rule 11.8 Example
// A cast shall not remove const or volatile qualification from the type pointed to by a pointer

#include <stddef.h>

// NON-COMPLIANT: Removing const qualifier
void non_compliant_const_removal(void) {
    const int x = 42;
    const int *const_ptr = &x;

    // Violation: Cast removes const qualifier
    int *ptr = (int *)const_ptr;  // automotive-c25-req-11.8
    *ptr = 10;  // Undefined behavior - modifying const object
}

// NON-COMPLIANT: Removing volatile qualifier
void non_compliant_volatile_removal(void) {
    volatile int hardware_register;
    volatile int *volatile_ptr = &hardware_register;

    // Violation: Cast removes volatile qualifier
    int *ptr = (int *)volatile_ptr;  // automotive-c25-req-11.8
    *ptr = 0;  // Compiler may optimize away necessary access
}

// NON-COMPLIANT: Removing both qualifiers
void non_compliant_both_removal(void) {
    const volatile int cv_value = 100;
    const volatile int *cv_ptr = &cv_value;

    // Violation: Cast removes both const and volatile
    int *ptr = (int *)cv_ptr;  // automotive-c25-req-11.8
}

// NON-COMPLIANT: Partial qualifier removal
void non_compliant_partial_removal(void) {
    const volatile int cv_value = 100;
    const volatile int *cv_ptr = &cv_value;

    // Violation: Cast removes volatile (keeps const)
    const int *c_ptr = (const int *)cv_ptr;  // automotive-c25-req-11.8

    // Violation: Cast removes const (keeps volatile)
    volatile int *v_ptr = (volatile int *)cv_ptr;  // automotive-c25-req-11.8
}

// COMPLIANT: Adding qualifiers is allowed
void compliant_add_qualifiers(void) {
    int x = 42;
    int *ptr = &x;

    // OK: Adding const qualifier
    const int *const_ptr = (const int *)ptr;

    // OK: Adding volatile qualifier
    volatile int *volatile_ptr = (volatile int *)ptr;

    // OK: Adding both qualifiers
    const volatile int *cv_ptr = (const volatile int *)ptr;
}

// COMPLIANT: Same qualifiers
void compliant_same_qualifiers(void) {
    const int x = 42;
    const int *const_ptr = &x;

    // OK: Same qualifiers
    const int *another_const_ptr = (const int *)const_ptr;
}

// COMPLIANT: Using pointers without casting away qualifiers
void compliant_usage(const int *const_ptr) {
    // OK: Direct assignment without cast
    const int *ptr2 = const_ptr;

    // OK: Reading through const pointer
    int value = *const_ptr;

    (void)ptr2;
    (void)value;
}

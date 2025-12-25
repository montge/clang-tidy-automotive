// RUN: %check_clang_tidy %s automotive-cpp23-mand-25.5 %t

#include <clocale>
#include <cstdlib>
#include <cstring>

// Simulated declarations for testing without full headers
extern "C" {
char *getenv(const char *name);
char *setlocale(int category, const char *locale);
struct lconv *localeconv();
char *strerror(int errnum);
}

// Test violations - non-const pointer assignments

void test_getenv_violations() {
    // Non-const pointer from getenv - violation
    char *path = getenv("PATH");
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: pointer returned by 'getenv' shall only be used as pointer to const-qualified type

    // Assignment to existing non-const pointer - violation
    char *home;
    home = getenv("HOME");
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: pointer returned by 'getenv' shall only be used as pointer to const-qualified type
}

void test_setlocale_violations() {
    // Non-const pointer from setlocale - violation
    char *loc = setlocale(0, "C");
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: pointer returned by 'setlocale' shall only be used as pointer to const-qualified type
}

void test_strerror_violations() {
    // Non-const pointer from strerror - violation
    char *err = strerror(0);
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: pointer returned by 'strerror' shall only be used as pointer to const-qualified type
}

void test_localeconv_violations() {
    // Non-const pointer from localeconv - violation
    struct lconv *lc = localeconv();
    // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: pointer returned by 'localeconv' shall only be used as pointer to const-qualified type
}

// Test compliant usage - const pointer assignments

void test_getenv_compliant() {
    // Const pointer from getenv - compliant
    const char *path = getenv("PATH");

    // Const pointer assignment - compliant
    const char *home;
    home = getenv("HOME");
}

void test_setlocale_compliant() {
    // Const pointer from setlocale - compliant
    const char *loc = setlocale(0, "C");
}

void test_strerror_compliant() {
    // Const pointer from strerror - compliant
    const char *err = strerror(0);
}

void test_localeconv_compliant() {
    // Const pointer from localeconv - compliant
    const struct lconv *lc = localeconv();
}

// Test direct usage without assignment (compliant)
void test_direct_usage() {
    // Direct usage in function call - compliant
    if (getenv("PATH") != nullptr) {
        // Do something
    }
}

// Test volatile const (compliant - const is present)
void test_volatile_const() {
    const volatile char *path = getenv("PATH");
}

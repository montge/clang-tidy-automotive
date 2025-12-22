// Test file for: automotive-c23-req-22.10
// Related MISRA C:2012 Rule: 22.10
//
// The value of errno shall only be tested when the last function
// to be called was an errno-setting-function.

// RUN: %check_clang_tidy %s automotive-c23-req-22.10 %t

#include <errno.h>
#include <stdlib.h>
#include <string.h>

// Non-errno-setting function
int some_function(void) { return 0; }

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_errno_after_non_setting_function(void) {
    int x = some_function();
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: errno tested after calling 'some_function' which is not an errno-setting function
    if (errno != 0) {
        // handle error
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_errno_after_strtol(const char *str) {
    char *endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);
    // Compliant: strtol is an errno-setting function
    if (errno != 0) {
        // handle error
    }
    (void)val;
}

void test_errno_after_strtod(const char *str) {
    char *endptr;
    errno = 0;
    double val = strtod(str, &endptr);
    // Compliant: strtod is an errno-setting function
    if (errno == ERANGE) {
        // handle range error
    }
    (void)val;
}

void test_errno_after_malloc(size_t size) {
    errno = 0;
    void *ptr = malloc(size);
    // Compliant: malloc is an errno-setting function
    if (ptr == NULL && errno == ENOMEM) {
        // handle out of memory
    }
    free(ptr);
}

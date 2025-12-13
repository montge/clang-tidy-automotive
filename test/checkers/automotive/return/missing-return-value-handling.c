// Test file for: automotive-missing-return-value-handling
// Related MISRA C:2025 Rule: 17.7
//
// This file tests the detection of ignored return values

// RUN: %check_clang_tidy %s automotive-missing-return-value-handling %t

// Declare functions that return values
int getStatus(void);
int processData(int x);
void *allocateMemory(int size);
int checkError(void);

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return value ignored
    getStatus();  // Return value discarded

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return value ignored
    processData(42);  // Return value discarded

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return value ignored
    allocateMemory(100);  // Return value discarded

    int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return value ignored
    checkError();  // Return value discarded
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void good_function1(void);  // void return type

void test_compliant(void) {
    // Void functions - no return value to check
    good_function1();

    // Return value used in assignment
    int status = getStatus();
    (void)status;

    // Return value used in condition
    if (processData(42) != 0) {
        // Handle error
    }

    // Return value used in expression
    void *ptr = allocateMemory(100);
    (void)ptr;

    // Explicit cast to void (intentional discard)
    (void)checkError();
}

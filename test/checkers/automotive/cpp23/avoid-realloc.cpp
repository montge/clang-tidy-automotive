// RUN: %check_clang_tidy %s automotive-cpp23-req-19.3 %t

// Test: realloc prohibition (MISRA C++:2023 Rule 19.3)
// Check ID: automotive-cpp23-req-19.3

// Forward declarations for stdlib functions
extern "C" {
void* malloc(unsigned long);
void* realloc(void*, unsigned long);
void free(void*);
}

namespace std {
void* malloc(unsigned long size) { return ::malloc(size); }
// CHECK-MESSAGES: :[[@LINE+1]]:55: warning: use of realloc is prohibited
void* realloc(void* ptr, unsigned long size) { return ::realloc(ptr, size); }
void free(void* ptr) { ::free(ptr); }
}

// ============= Violations: realloc usage =============

void test_realloc_direct() {
    void* ptr = malloc(10);
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: use of realloc is prohibited
    ptr = realloc(ptr, 20);
    free(ptr);
}

void test_realloc_std() {
    void* ptr = std::malloc(10);
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: use of realloc is prohibited
    ptr = std::realloc(ptr, 20);
    std::free(ptr);
}

void test_realloc_null() {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: use of realloc is prohibited
    void* ptr = realloc(nullptr, 10);
    free(ptr);
}

void test_realloc_shrink() {
    void* ptr = malloc(100);
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: use of realloc is prohibited
    ptr = realloc(ptr, 50);
    free(ptr);
}

// ============= Compliant: Using malloc/free properly =============

void test_compliant_malloc_free() {
    void* ptr = malloc(20);  // OK - direct allocation
    free(ptr);
}

void test_compliant_std_malloc_free() {
    void* ptr = std::malloc(20);  // OK - direct allocation
    std::free(ptr);
}

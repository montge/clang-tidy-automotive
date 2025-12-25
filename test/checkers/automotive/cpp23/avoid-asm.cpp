// RUN: %check_clang_tidy %s automotive-cpp23-req-7.0.1 %t -- -- -std=gnu++14
// Test for automotive-cpp23-req-7.0.1: asm declaration prohibition
// Related MISRA C++:2023 Rule: 7.0.1

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_standard_asm() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    asm("nop");
}

void test_gcc_asm_simple() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__("nop");
}

void test_gcc_asm_volatile() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__ volatile("nop");
}

void test_gcc_asm_with_output() {
    int output;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__("mov %1, %0" : "=r"(output) : "r"(42));
}

void test_gcc_asm_with_input_output() {
    int input = 10;
    int output;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__("add %2, %0" : "=r"(output) : "r"(input), "r"(5));
}

void test_gcc_asm_with_clobbers() {
    int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__("incl %0" : "+r"(x) : : "cc");
}

void test_multiline_asm() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    asm("movl $1, %eax\n"
        "movl $0, %ebx\n"
        "int $0x80");
}

// Test inline assembly in function
int test_asm_with_return() {
    int result;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__("mov $42, %0" : "=r"(result));
    return result;
}

// Multiple asm statements in one function
void test_multiple_asm() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    asm("nop");
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__("nop");
}

// Test with goto labels (GCC extended asm)
void test_asm_goto() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__ goto("jmp %l[label]" : : : : label);
label:
    return;
}

// Inline assembly to access CPU features
void test_cpu_feature_asm() {
    unsigned int eax, ebx, ecx, edx;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: the asm declaration shall not be used [automotive-cpp23-req-7.0.1]
    __asm__("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0));
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Regular C++ function (no assembly)
void compliant_function() {
    int x = 42;
    int y = x + 10;
}

// Using compiler intrinsics instead of inline assembly
#ifdef __x86_64__
#include <immintrin.h>
void compliant_intrinsics() {
    // Using compiler intrinsics is compliant (not inline asm)
    _mm_pause();
}
#endif

// Using standard C++ atomic operations instead of inline assembly
#include <atomic>
void compliant_atomic() {
    std::atomic<int> counter{0};
    counter.fetch_add(1, std::memory_order_relaxed);
}

// Regular function with complex logic
int compliant_complex() {
    int result = 0;
    for (int i = 0; i < 10; i++) {
        result += i;
    }
    return result;
}

// Using standard library functions instead of assembly
#include <cstring>
void compliant_memcpy() {
    char src[10] = "test";
    char dst[10];
    std::memcpy(dst, src, sizeof(src));
}

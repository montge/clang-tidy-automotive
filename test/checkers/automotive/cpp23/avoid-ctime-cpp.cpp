// RUN: %check_clang_tidy %s automotive-cpp23-req-21.10 %t
// Test for automotive-cpp23-req-21.10: ctime prohibition
// Related MISRA C++:2023 Rule: 21.10

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: <ctime> header shall not be used; use <chrono> [automotive-cpp23-req-21.10]
#include <ctime>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_ctime_functions() {
    std::time_t now;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: time from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    now = std::time(nullptr);

    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: ctime from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    const char *s = std::ctime(&now);
    (void)s;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: localtime from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    std::tm *local = std::localtime(&now);
    (void)local;

    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: gmtime from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    std::tm *gmt = std::gmtime(&now);
    (void)gmt;

    char buffer[26];
    std::tm timeinfo{};
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: asctime from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    std::asctime(&timeinfo);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: strftime from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: mktime from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    std::mktime(&timeinfo);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: difftime from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    std::difftime(now, now);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: clock from <ctime> shall not be used; use <chrono> [automotive-cpp23-req-21.10]
    std::clock();
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Use <chrono> for time operations in C++
#include <chrono>

void test_chrono() {
    // Use std::chrono instead of ctime
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch());
    (void)duration;

    // High resolution timing
    auto start = std::chrono::high_resolution_clock::now();
    // ... do work ...
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    (void)elapsed;
}

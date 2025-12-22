// RUN: %check_clang_tidy %s automotive-cpp23-req-21.10 %t -- -- -std=c++11

// Test for automotive-cpp23-req-21.10: ctime prohibition
// Related MISRA C++:2023 Rule: 21.10
//
// Note: Using forward declarations to avoid -nostdinc++ issues with headers.
// The check also detects #include <ctime> via preprocessor callbacks.

// Forward declarations to simulate <ctime> types
typedef unsigned long size_t;

namespace std {
typedef long time_t;
typedef long clock_t;
struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

// Forward declarations of ctime functions
time_t time(time_t* timer);
clock_t clock();
double difftime(time_t end, time_t beginning);
time_t mktime(tm* timeptr);
char* asctime(const tm* timeptr);
char* ctime(const time_t* timer);
tm* gmtime(const time_t* timer);
tm* localtime(const time_t* timer);
size_t strftime(char* ptr, size_t maxsize, const char* format, const tm* timeptr);
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_ctime_functions() {
    std::time_t now;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: time from <ctime> shall not be used; use <chrono>
    now = std::time(nullptr);

    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: ctime from <ctime> shall not be used; use <chrono>
    const char *s = std::ctime(&now);
    (void)s;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: localtime from <ctime> shall not be used; use <chrono>
    std::tm *local = std::localtime(&now);
    (void)local;

    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: gmtime from <ctime> shall not be used; use <chrono>
    std::tm *gmt = std::gmtime(&now);
    (void)gmt;

    char buffer[26];
    std::tm timeinfo{};
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: asctime from <ctime> shall not be used; use <chrono>
    std::asctime(&timeinfo);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: strftime from <ctime> shall not be used; use <chrono>
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: mktime from <ctime> shall not be used; use <chrono>
    std::mktime(&timeinfo);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: difftime from <ctime> shall not be used; use <chrono>
    std::difftime(now, now);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: clock from <ctime> shall not be used; use <chrono>
    std::clock();
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Use <chrono> instead (not tested here due to -nostdinc++)
// std::chrono::system_clock, std::chrono::duration, etc.

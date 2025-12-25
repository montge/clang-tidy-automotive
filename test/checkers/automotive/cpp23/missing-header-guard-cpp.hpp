// RUN: %check_clang_tidy %s automotive-cpp23-req-19.2.1 %t -- -- -std=c++17

// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: header file is missing header guard

// This header lacks proper include guards
class MyClass {
public:
    void doSomething();
};

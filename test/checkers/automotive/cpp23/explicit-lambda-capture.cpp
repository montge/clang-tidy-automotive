// RUN: %check_clang_tidy -std=c++14-or-later %s automotive-cpp23-adv-8.1 %t
// Test for automotive-cpp23-adv-8.1
// Related MISRA C++:2023 Rule: 8.1

// This test verifies that lambda expressions with default captures are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_default_capture_by_copy() {
    int x = 10;
    int y = 20;

    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: lambda uses default capture by copy '[=]'; explicitly list all captured variables instead
    auto lambda = [=]() { return x + y; };
    (void)lambda;
}

void test_default_capture_by_ref() {
    int x = 10;
    int y = 20;

    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: lambda uses default capture by reference '[&]'; explicitly list all captured variables instead
    auto lambda = [&]() { x = y; };
    (void)lambda;
}

void test_default_with_explicit() {
    int x = 10;
    int y = 20;
    int z = 30;

    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: lambda uses default capture by copy '[=]'; explicitly list all captured variables instead
    auto lambda1 = [=, &z]() { return x + y + z; };

    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: lambda uses default capture by reference '[&]'; explicitly list all captured variables instead
    auto lambda2 = [&, z]() { x = y + z; };

    (void)lambda1;
    (void)lambda2;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_explicit_captures() {
    int x = 10;
    int y = 20;

    // Explicit by-copy capture
    auto lambda1 = [x, y]() { return x + y; };

    // Explicit by-reference capture
    auto lambda2 = [&x, &y]() { x = y; };

    // Mixed explicit captures
    auto lambda3 = [x, &y]() { return x + y; };

    // No captures
    auto lambda4 = []() { return 42; };

    (void)lambda1;
    (void)lambda2;
    (void)lambda3;
    (void)lambda4;
}

void test_this_capture() {
    struct MyClass {
        int value;
        void method() {
            // Explicit this capture is OK
            auto lambda = [this]() { return value; };
            (void)lambda;
        }
    };
}

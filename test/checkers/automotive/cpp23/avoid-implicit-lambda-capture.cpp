// RUN: %check_clang_tidy %s automotive-cpp23-req-0.1 %t
// Test for automotive-cpp23-req-0.1
// Related MISRA C++:2023 Rule: 0.1

// This test verifies that implicit lambda captures are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_implicit_capture_by_copy() {
    int x = 1, y = 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: lambda uses implicit capture-by-copy [=]
    auto f = [=]() { return x + y; };

    (void)f;
}

void test_implicit_capture_by_ref() {
    int x = 1, y = 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: lambda uses implicit capture-by-reference [&]
    auto f = [&]() { return x + y; };

    (void)f;
}

void test_implicit_with_explicit() {
    int x = 1, y = 2;

    // Even when some captures are explicit, implicit default is still a problem
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: lambda uses implicit capture-by-copy [=]
    auto f = [=, &x]() { return x + y; };

    (void)f;
}

void test_implicit_by_ref_with_explicit() {
    int x = 1, y = 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: lambda uses implicit capture-by-reference [&]
    auto f = [&, x]() { return x + y; };

    (void)f;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_explicit_capture_by_copy() {
    int x = 1, y = 2;

    // All captures are explicit - no warning
    auto f = [x, y]() { return x + y; };

    (void)f;
}

void test_explicit_capture_by_ref() {
    int x = 1, y = 2;

    // All captures are explicit - no warning
    auto f = [&x, &y]() { return x + y; };

    (void)f;
}

void test_mixed_explicit_capture() {
    int x = 1, y = 2;

    // Mixed explicit captures - no warning
    auto f = [x, &y]() { return x + y; };

    (void)f;
}

void test_no_capture() {
    // Lambda with no captures - no warning
    auto f = []() { return 42; };

    (void)f;
}

void test_capture_this_explicit() {
    struct Foo {
        int val = 0;
        void bar() {
            // Explicit this capture - no warning
            auto f = [this]() { return val; };
            (void)f;
        }
    };
}

void test_init_capture() {
    int x = 1;

    // Init capture - no warning (explicit)
    auto f = [y = x]() { return y; };

    (void)f;
}

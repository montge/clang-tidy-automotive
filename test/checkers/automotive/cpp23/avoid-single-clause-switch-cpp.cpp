// RUN: %check_clang_tidy %s automotive-cpp23-req-9.4 %t
// Test for automotive-cpp23-req-9.4: Switch minimum clauses
// Related MISRA C++:2023 Rule: 9.4

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_single_case(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement shall have at least two non-empty switch-clauses [automotive-cpp23-req-9.4]
    switch (x) {
        case 1:
            break;
    }
}

void test_only_default(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement shall have at least two non-empty switch-clauses [automotive-cpp23-req-9.4]
    switch (x) {
        default:
            break;
    }
}

void test_empty_switch(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement shall have at least two non-empty switch-clauses [automotive-cpp23-req-9.4]
    switch (x) {
    }
}

enum class Color { Red, Green, Blue };

void test_enum_single_case(Color c) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement shall have at least two non-empty switch-clauses [automotive-cpp23-req-9.4]
    switch (c) {
        case Color::Red:
            break;
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_two_cases(int x) {
    switch (x) {
        case 1:
            break;
        case 2:
            break;
    }
}

void test_case_and_default(int x) {
    switch (x) {
        case 1:
            break;
        default:
            break;
    }
}

void test_enum_multiple_cases(Color c) {
    switch (c) {
        case Color::Red:
        case Color::Green:
        case Color::Blue:
            break;
    }
}

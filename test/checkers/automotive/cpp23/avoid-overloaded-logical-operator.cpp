// RUN: %check_clang_tidy %s automotive-cpp23-adv-16.5 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-16.5
// Related MISRA C++:2023 Rule: 16.5

// This test verifies that overloaded operator&& and operator|| are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

class BadClass {
public:
    int value;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: overloading operator&& disables short-circuit evaluation
    bool operator&&(const BadClass& other) const {
        return value && other.value;
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: overloading operator|| disables short-circuit evaluation
    bool operator||(const BadClass& other) const {
        return value || other.value;
    }
};

// Non-member overloads
struct AnotherBad {
    int val;
};

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: overloading operator&& disables short-circuit evaluation
bool operator&&(const AnotherBad& a, const AnotherBad& b) {
    return a.val && b.val;
}

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: overloading operator|| disables short-circuit evaluation
bool operator||(const AnotherBad& a, const AnotherBad& b) {
    return a.val || b.val;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

class GoodClass {
public:
    int value;

    // Other operators are fine
    bool operator==(const GoodClass& other) const {
        return value == other.value;
    }

    bool operator!=(const GoodClass& other) const {
        return value != other.value;
    }

    bool operator<(const GoodClass& other) const {
        return value < other.value;
    }

    // Bitwise operators are OK (they don't have short-circuit semantics)
    int operator&(const GoodClass& other) const {
        return value & other.value;
    }

    int operator|(const GoodClass& other) const {
        return value | other.value;
    }
};

void test_usage() {
    BadClass a{1}, b{2};
    // Using the overloaded operators (the definition triggers warning, not usage)
    bool r1 = a && b;
    bool r2 = a || b;

    GoodClass c{1}, d{2};
    bool r3 = c == d;
    int r4 = c & d;

    (void)r1;
    (void)r2;
    (void)r3;
    (void)r4;
}

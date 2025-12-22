// RUN: %check_clang_tidy %s automotive-cpp23-req-12.3 %t -- -- -std=c++11
// Test for automotive-cpp23-req-12.3: Union prohibition
// Related MISRA C++:2023 Rule: 12.3

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: union shall not be used [automotive-cpp23-req-12.3]
union BadUnion {
    int i;
    float f;
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: union shall not be used [automotive-cpp23-req-12.3]
union AnonymousStyleUnion {
    char c;
    int i;
};

class ContainsUnion {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: union shall not be used [automotive-cpp23-req-12.3]
    union {
        int x;
        float y;
    } data;
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Use std::variant instead of union (in C++17)
// Regular struct
struct RegularStruct {
    int x;
    float y;
};

// Regular class
class RegularClass {
    int x;
    float y;
};

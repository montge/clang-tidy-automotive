// RUN: %check_clang_tidy %s automotive-cpp23-adv-5.7.1 %t -- -- -std=c++17

// Test: Tag name hiding (MISRA C++:2023 Rule 5.7 Advisory)
// Check ID: automotive-cpp23-adv-5.7.1

// ============= Global scope =============

int Point = 5;
int Color = 10;

// ============= Violations: Tag names hiding variables =============

void test_hiding_global_variable() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: tag name 'Point' hides global/namespace-scope variable
    struct Point {
        int x, y;
    };
}

// Namespace scope test
namespace ns {
    int Shape = 20;
}

void test_hiding_namespace_variable() {
    // Note: This might not be caught due to namespace scoping
}

// ============= Hiding function names =============

void Rectangle() {
    // Function exists
}

void test_hiding_function() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: tag name 'Rectangle' hides function in enclosing scope
    struct Rectangle {
        int width, height;
    };
}

// ============= Hiding parameters =============

void test_hiding_parameter(int Size) {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: tag name 'Size' hides parameter in enclosing function
    struct Size {
        int bytes;
    };
}

// ============= Hiding member variables =============

class Container {
public:
    int Data;

    void method() {
        // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: tag name 'Data' hides member of enclosing class
        struct Data {
            int value;
        };
    }
};

// ============= Hiding typedef =============

typedef int MyInt;

void test_hiding_typedef() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: tag name 'MyInt' hides typedef in enclosing scope
    struct MyInt {
        int val;
    };
}

// ============= Hiding enumerator =============

enum GlobalEnum {
    EnumValue = 100
};

void test_hiding_enumerator() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: tag name 'EnumValue' hides enumerator in enclosing scope
    struct EnumValue {
        int data;
    };
}

// ============= Compliant: No hiding =============

void compliant_no_hiding() {
    struct Location {  // OK - unique name
        int x, y;
    };
}

void compliant_different_name(int param) {
    struct MyStruct {  // OK - different name
        int field;
    };
}

// Compliant class definition
class CompliantClass {
public:
    int member;

    void method() {
        struct LocalStruct {  // OK - unique name
            int value;
        };
    }
};

// ============= Edge cases =============

// Multiple scopes - variables in function scope
void nested_scopes() {
    int outer = 1;
    {
        // The function scope is still an enclosing scope
        // Note: This case might not be caught depending on how scopes are traversed
        int inner = 2;
    }
}

// Enum tag hiding
void enum_hiding() {
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: tag name 'Color' hides global/namespace-scope variable
    enum Color {
        RED, GREEN, BLUE
    };
}

// Union tag hiding
void union_hiding() {
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: tag name 'Point' hides global/namespace-scope variable
    union Point {
        int i;
        float f;
    };
}

// Class tag hiding
void class_hiding() {
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: tag name 'Color' hides global/namespace-scope variable
    class Color {
        int rgb;
    };
}

// Anonymous tag should not trigger
void anonymous_tag() {
    struct {  // OK - anonymous, no name to hide
        int x, y;
    } point;
}

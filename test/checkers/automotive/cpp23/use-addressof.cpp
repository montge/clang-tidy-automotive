// RUN: %check_clang_tidy -std=c++14-or-later %s automotive-cpp23-adv-19.0 %t
// Test for automotive-cpp23-adv-19.0
// Related MISRA C++:2023 Rule: 19.0

// This test verifies that using & on class types is detected.

// Mock std::addressof to avoid needing <memory> header
namespace std {
template<typename T>
T* addressof(T& arg) noexcept {
    return reinterpret_cast<T*>(
        &const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
}
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

struct MyClass {
    int value;
};

void test_address_of_class() {
    MyClass obj;
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: use 'std::addressof' instead of unary '&' for class type 'MyClass' to avoid issues with overloaded operator&
    MyClass* ptr1 = &obj;
    (void)ptr1;
}

struct AnotherClass {
    int value;
    double data;
};

void test_address_of_another() {
    AnotherClass obj;
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: use 'std::addressof' instead of unary '&' for class type 'AnotherClass' to avoid issues with overloaded operator&
    AnotherClass* ptr = &obj;
    (void)ptr;
}

struct ClassWithOverloadedOp {
    int value;
    // This class has operator& overloaded - not flagged because it's intentional
    ClassWithOverloadedOp* operator&() { return nullptr; }
};

void test_address_of_overloaded() {
    ClassWithOverloadedOp obj;
    // When operator& is overloaded, Clang treats it as CXXOperatorCallExpr
    // This is actually intentional use, so we don't flag it
    ClassWithOverloadedOp* ptr = &obj;
    (void)ptr;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_primitive_types() {
    int x = 10;
    int* ptr = &x;  // OK - primitive type

    double d = 3.14;
    double* dp = &d;  // OK - primitive type

    (void)ptr;
    (void)dp;
}

void test_pointer_types() {
    int x = 10;
    int* ptr = &x;
    int** pptr = &ptr;  // OK - pointer type

    (void)pptr;
}

void test_array_element() {
    int arr[10];
    int* ptr = &arr[0];  // OK - array element

    (void)ptr;
}

// Using std::addressof is the correct way
void test_std_addressof() {
    MyClass obj;
    MyClass* ptr = std::addressof(obj);  // OK - using std::addressof
    (void)ptr;
}

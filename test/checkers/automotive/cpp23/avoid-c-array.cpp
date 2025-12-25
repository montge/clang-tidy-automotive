// RUN: %check_clang_tidy %s automotive-cpp23-adv-11.3.1 %t -- -- -std=c++11
// Test for automotive-cpp23-adv-11.3.1: C-style array prohibition
// Related MISRA C++:2023 Rule: 11.3.1

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: C-style array 'arr' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
int arr[10];

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: C-style array 'buffer' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
char buffer[256];

// Multi-dimensional arrays
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: C-style array 'matrix' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
int matrix[3][3];

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: C-style array 'cube' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
long cube[2][3][4];

// Array parameters in functions
// CHECK-MESSAGES: :[[@LINE+1]]:22: warning: C-style array 'arr' should be replaced with std::span, std::vector, or a reference to std::array for better safety [automotive-cpp23-adv-11.3.1]
void processArray(int arr[]) {
    // Function body
}

// CHECK-MESSAGES: :[[@LINE+1]]:29: warning: C-style array 'data' should be replaced with std::span, std::vector, or a reference to std::array for better safety [automotive-cpp23-adv-11.3.1]
void processArray2(const int data[10]) {
    // Function body
}

// CHECK-MESSAGES: :[[@LINE+1]]:27: warning: C-style array 'matrix' should be replaced with std::span, std::vector, or a reference to std::array for better safety [automotive-cpp23-adv-11.3.1]
void processMatrix(double matrix[3][3]) {
    // Function body
}

// Local arrays inside functions
void localArrayFunction() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: C-style array 'localArr' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
    int localArr[5];

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: C-style array 'temp' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
    char temp[100];
}

// Class member arrays
class TestClass {
public:
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: C-style array 'memberArray' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
    int memberArray[20];

private:
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: C-style array 'privateArray' should be replaced with std::array or std::vector for better safety [automotive-cpp23-adv-11.3.1]
    char privateArray[50];
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Main function's argv parameter should be allowed
int main(int argc, char* argv[]) {
    return 0;
}

// Another valid main signature
int main(int argc, char** argv) {
    return 0;
}

// extern "C" contexts should be allowed for C interoperability
extern "C" {
    void c_function(int arr[10]);
    int c_array[100];
}

// extern "C" function
extern "C" void another_c_function(char buffer[]) {
    // Function body
}

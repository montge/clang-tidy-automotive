// RUN: %check_clang_tidy %s automotive-cpp23-adv-16.6 %t

// MISRA C++:2023 Rule 16.6 (Advisory): A non-type template parameter of
// reference type should not bind to a temporary.
//
// All test cases are compliant - no warnings expected.
// Note: Most violations result in compilation errors in standard C++.

// Template with const reference parameter
template<const int& N>
struct S {
  int getValue() { return N; }
};

// Template with non-const reference parameter
template<int& N>
struct T {
  int getValue() { return N; }
};

// Global variables
int global_x = 42;
static int static_x = 42;
const int const_global = 100;

// OK - reference to global variable
S<global_x> s1;

// OK - reference to static variable
S<static_x> s2;

// OK - const reference to const global
S<const_global> s3;

// OK - static member variable
struct Data {
  static int value;
};
int Data::value = 50;
S<Data::value> s4;

// OK - extern variable
extern int extern_var;
int extern_var = 77;
S<extern_var> s5;

// OK - namespace variable
namespace NS {
  int ns_var = 88;
}
S<NS::ns_var> s6;

// OK - pointer template (not a reference)
template<int* P>
struct PtrHolder {
  int getValue() { return *P; }
};
PtrHolder<&global_x> ptr1;

// OK - array reference
int global_array[5] = {1, 2, 3, 4, 5};
template<int (&Arr)[5]>
struct ArrayRef {
  int getFirst() { return Arr[0]; }
};
ArrayRef<global_array> arr1;

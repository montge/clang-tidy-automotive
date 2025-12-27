// RUN: %check_clang_tidy %s automotive-cpp23-adv-16.6 %t

// MISRA C++:2023 Rule 16.6 (Advisory): A non-type template parameter of
// reference type should not bind to a temporary.
//
// All test cases are compliant - no warnings expected.

// Template with non-type reference parameter
template<int& N>
struct Holder {
  int getValue() { return N; }
};

// Template with const reference parameter
template<const int& N>
struct ConstHolder {
  int getValue() { return N; }
};

// Global variables
int global_int = 42;
const int const_global_int = 100;
static int static_int = 99;

// OK - bind to lvalue (global variable)
Holder<global_int> h1;

// OK - bind to static variable
Holder<static_int> h2;

// OK - const reference to global
ConstHolder<const_global_int> ch1;

// OK - reference to static member
struct Data {
  static int member;
};
int Data::member = 50;
Holder<Data::member> h4;

// OK - extern variable
extern int extern_int;
int extern_int = 88;
Holder<extern_int> h6;

// OK - pointer template (not a reference)
template<int* P>
struct PointerHolder {
  int getValue() { return *P; }
};
PointerHolder<&global_int> ph1;

// OK - array reference
int global_array[10] = {0};
template<int (&Arr)[10]>
struct ArrayHolder {
  int getFirst() { return Arr[0]; }
};
ArrayHolder<global_array> ah1;

// OK - namespace-scope variable
namespace NS {
  int ns_var = 77;
}
Holder<NS::ns_var> h8;

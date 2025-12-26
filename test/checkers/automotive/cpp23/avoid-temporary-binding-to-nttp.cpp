// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-adv-16.6 %t

// MISRA C++:2023 Rule 16.6 (Advisory): A non-type template parameter of
// reference type should not bind to a temporary.
//
// NOTE: In standard C++, most violations of this rule result in compilation
// errors, as temporaries cannot bind to non-const lvalue references, and
// binding to const lvalue references in template arguments is typically
// rejected by the compiler. This check is designed to catch edge cases
// and provide additional safety.

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

// Template function with const reference parameter
template<const int& N>
void processConstRef() {
  // Use N
}

// Template function with reference parameter
template<int& N>
void processRef() {
  // Use N
}

// Global variables (compliant)
int global_x = 42;
static int static_x = 42;
const int const_global = 100;

// Test case 1: Compliant - reference to named variable
int x = 42;
S<x> s2;  // OK

// Test case 2: Compliant - reference to static variable
S<static_x> s3;  // OK

// Test case 3: Compliant - reference to global variable
S<global_x> s4;  // OK

// Test case 4: Compliant - non-reference template parameter with temporary
template<int N>
struct U {
  int getValue() { return N; }
};

U<42> u1;  // OK - not a reference parameter

// Test case 5: Compliant - constexpr variable
constexpr int constexpr_val = 100;
S<constexpr_val> s6;  // OK - constexpr has static storage

// Test case 6: Compliant - reference to const global
S<const_global> s7;  // OK

// Test case 7: Compliant - static member variable
struct Data {
  static int value;
};
int Data::value = 50;
S<Data::value> s9;  // OK

// Test case 8: Compliant - extern variable
extern int extern_var;
int extern_var = 77;
S<extern_var> s10;  // OK

// Test case 9: Compliant - function with named variable
void test_function_ok() {
  processRef<global_x>();  // OK - global variable
}

// Test case 10: Compliant - namespace variable
namespace NS {
  int ns_var = 88;
}
S<NS::ns_var> s12;  // OK

// Test case 11: Template with multiple reference parameters - both compliant
template<const int& A, const int& B>
struct TwoRefs {
  int sum() { return A + B; }
};

int y = 10;
TwoRefs<global_x, y> tr3;  // OK

// Test case 12: Compliant - inline variable (C++17)
inline int inline_var = 123;
S<inline_var> s13;  // OK - inline variables have static storage

// Test case 13: Compliant - anonymous namespace variable
namespace {
  int anon_var = 55;
}
S<anon_var> s14;  // OK

// Test case 14: Template with auto reference (C++17) - compliant
template<auto& N>
struct AutoRef {
  decltype(N) getValue() { return N; }
};

AutoRef<global_x> ar1;  // OK - binds to lvalue

// Test case 15: Nested template instantiation - compliant
template<const int& R>
struct Outer {
  template<const int& S>
  struct Inner {
    int get() { return R + S; }
  };
};

Outer<global_x>::Inner<static_x> nested1;  // OK - both lvalues

// Test case 16: Alias template - compliant
template<const int& N>
using SAlias = S<N>;

SAlias<global_x> alias1;  // OK

// Test case 17: Pointer parameter (not a reference - compliant)
template<int* P>
struct PtrHolder {
  int getValue() { return *P; }
};

PtrHolder<&global_x> ptr1;  // OK - pointer, not reference

// Test case 18: Array reference parameter - compliant
int global_array[5] = {1, 2, 3, 4, 5};

template<int (&Arr)[5]>
struct ArrayRef {
  int getFirst() { return Arr[0]; }
};

ArrayRef<global_array> arr_ref1;  // OK - array lvalue

// Test case 19: Reference to static constexpr member
struct Constants {
  static constexpr int value = 300;
};

S<Constants::value> s15;  // OK - static constexpr member

// Test case 20: Class template with mixed parameters
template<typename T, const int& N>
class Container {
  T value;
public:
  Container(T v) : value(v) {}
  int getN() { return N; }
};

Container<double, global_x> c1(3.14);  // OK

// Test case 21: Function pointer template (not a reference)
void someFunction() {}

template<void (*F)()>
struct FunctionHolder {
  void call() { F(); }
};

FunctionHolder<&someFunction> fh1;  // OK - function pointer

// Test case 22: Trailing return type with decltype
template<int& N>
auto getRefAuto() -> decltype(N) {
  return N;
}

void test_auto_return() {
  int& ref = getRefAuto<global_x>();  // OK
}

// Test case 23: Variable template
template<int& N>
int& getRef() {
  return N;
}

void test_var_template() {
  int& ref1 = getRef<global_x>();  // OK
}

// Note: Most violations (like S<42>) result in compilation errors in C++
// and cannot be demonstrated in compilable test code. The compiler's own
// diagnostics catch these cases before clang-tidy runs.

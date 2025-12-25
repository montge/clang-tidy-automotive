// RUN: %check_clang_tidy %s automotive-cpp23-adv-16.6 %t

// MISRA C++:2023 Rule 16.6 (Advisory): A non-type template parameter of
// reference type should not bind to a temporary.
//
// NOTE: Most violations of this rule result in compilation errors in standard
// C++, as temporaries cannot bind to non-const lvalue references. The cases
// where temporaries CAN bind (const lvalue references or rvalue references)
// are implementation-defined and compiler-dependent.

// Template with non-type reference parameter
template<int& N>
struct Holder {
  int getValue() { return N; }
};

// Template function with reference parameter
template<int& N>
void processRef() {
  // Use N
}

// Template with const reference parameter
template<const int& N>
struct ConstHolder {
  int getValue() { return N; }
};

template<const int& N>
void processConstRef() {
  // Use N
}

// Global variables
int global_int = 42;
const int const_global_int = 100;

// Static variable
static int static_int = 99;

// Test case 1: OK - bind to lvalue (global variable)
Holder<global_int> h1;  // OK: binds to lvalue

// Test case 2: OK - bind to static variable
Holder<static_int> h2;  // OK: binds to static variable

// Test case 3: OK - const reference to global
ConstHolder<const_global_int> ch1;  // OK: binds to global const

// Test case 4: OK - function call with lvalue
void test_function_ok() {
  processRef<global_int>();  // OK: binds to lvalue
  processConstRef<const_global_int>();  // OK: binds to const lvalue
}

// Test case 5: constexpr variable (OK - has static storage)
constexpr int constexpr_value = 42;
ConstHolder<constexpr_value> ch3;  // OK: constexpr has static storage

// Test case 6: Variable template with reference parameter
template<int& N>
int& getRef() {
  return N;
}

void test_var_template() {
  int& ref1 = getRef<global_int>();  // OK: binds to lvalue
}

// Test case 7: Class template with multiple parameters
template<typename T, int& N>
class Container {
  T value;
public:
  Container(T v) : value(v) {}
  int getN() { return N; }
};

Container<double, global_int> c1(3.14);  // OK: reference binds to lvalue

// Test case 8: Nested template instantiation
template<int& R>
struct Outer {
  template<int& S>
  struct Inner {
    int get() { return R + S; }
  };
};

Outer<global_int>::Inner<static_int> nested;  // OK: both bind to lvalues

// Test case 9: Template with multiple reference parameters
template<int& A, int& B>
struct TwoRefs {
  int sum() { return A + B; }
};

int second_global = 20;
TwoRefs<global_int, second_global> tr1;  // OK: both bind to lvalues

// Test case 10: Pointer vs reference (pointers are OK)
template<int* P>
struct PointerHolder {
  int getValue() { return *P; }
};

PointerHolder<&global_int> ph1;  // OK: pointer to lvalue

// Test case 11: Array reference
int global_array[10] = {0};

template<int (&Arr)[10]>
struct ArrayHolder {
  int getFirst() { return Arr[0]; }
};

ArrayHolder<global_array> ah1;  // OK: array reference binds to lvalue

// Test case 12: Function pointer template (not a reference)
void someFunction() {}

template<void (*F)()>
struct FunctionHolder {
  void call() { F(); }
};

FunctionHolder<&someFunction> fh1;  // OK: function pointer, not reference

// Test case 13: Reference to member
struct Data {
  static int member;
};
int Data::member = 50;

Holder<Data::member> h4;  // OK: static member

// Test case 14: Extern variable (OK)
extern int extern_int;
int extern_int = 88;
Holder<extern_int> h6;  // OK: extern variable

// Test case 15: Inline variable (C++17, OK - has static storage)
inline int inline_var = 123;
Holder<inline_var> h7;  // OK: inline variable has static storage

// Test case 16: Constant expression with static storage
struct Constants {
  static constexpr int value = 300;
};
constexpr int Constants::value;  // Definition

ConstHolder<Constants::value> ch5;  // OK: static constexpr member

// Test case 17: NTTP deduction (C++17)
template<auto& N>
struct AutoRefHolder {
  decltype(N) getValue() { return N; }
};

AutoRefHolder<global_int> arh1;  // OK: deduced reference binds to lvalue
AutoRefHolder<const_global_int> arh2;  // OK: const reference to global

// Test case 18: Trailing return type with decltype
template<int& N>
auto getRefAuto() -> decltype(N) {
  return N;
}

void test_auto_return() {
  int& ref = getRefAuto<global_int>();  // OK
}

// Test case 19: Alias template
template<int& N>
using RefAlias = Holder<N>;

RefAlias<global_int> ra1;  // OK: alias still binds to lvalue

// Test case 20: Namespace-scope variable
namespace NS {
  int ns_var = 77;
}

Holder<NS::ns_var> h8;  // OK: namespace-scope variable

// Test case 21: Anonymous namespace variable
namespace {
  int anon_var = 66;
}

Holder<anon_var> h9;  // OK: anonymous namespace variable

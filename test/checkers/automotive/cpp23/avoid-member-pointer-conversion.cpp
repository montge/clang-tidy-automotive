// RUN: %check_clang_tidy -std=c++11 %s automotive-cpp23-req-11.6 %t
// Test for automotive-cpp23-req-11.6: Invalid conversions between pointers
// to different member types should be avoided
// Note: static_cast between unrelated classes is a compiler error, so only
// reinterpret_cast is tested.

class Base {
public:
  int base_field;
  void base_method() {}
};

class Derived : public Base {
public:
  int derived_field;
};

class Unrelated {
public:
  int unrelated_field;
  void unrelated_method() {}
};

// Test reinterpret_cast between member pointers (always forbidden)
void test_reinterpret_cast() {
  int Base::*pb = &Base::base_field;

  // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: reinterpret_cast used to convert pointer-to-member from 'Base' to 'Unrelated'; invalid member pointer conversion
  int Unrelated::*pu = reinterpret_cast<int Unrelated::*>(pb);
  (void)pu;

  int Derived::*pd = &Derived::derived_field;

  // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: reinterpret_cast used to convert pointer-to-member from 'Derived' to 'Base'; invalid member pointer conversion
  int Base::*pb2 = reinterpret_cast<int Base::*>(pd);
  (void)pb2;
}

// Test member function pointers
void test_member_function_pointers() {
  void (Base::*pmb)() = &Base::base_method;

  // CHECK-MESSAGES: :[[@LINE+1]]:30: warning: reinterpret_cast used to convert pointer-to-member from 'Base' to 'Unrelated'; invalid member pointer conversion
  void (Unrelated::*pmu)() = reinterpret_cast<void (Unrelated::*)()>(pmb);
  (void)pmu;
}

// Valid conversions (should not warn)
void test_valid_conversions() {
  // Derived to base is valid
  int Derived::*pd = &Derived::derived_field;
  int Base::*pb = static_cast<int Base::*>(pd);  // OK
  (void)pb;

  // Same class conversion
  int Base::*pb1 = &Base::base_field;
  int Base::*pb2 = static_cast<int Base::*>(pb1);  // OK
  (void)pb2;
}

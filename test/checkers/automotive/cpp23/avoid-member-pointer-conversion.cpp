// RUN: %check_clang_tidy %s automotive-cpp23-req-11.6 %t
// Test for automotive-cpp23-req-11.6: Invalid conversions between pointers
// to different member types should be avoided

class Base {
public:
  int base_field;
  void base_method() {}
};

class Derived : public Base {
public:
  int derived_field;
  void derived_method() {}
};

class Unrelated {
public:
  int unrelated_field;
  void unrelated_method() {}
};

// Test reinterpret_cast between member pointers (always forbidden)
void test_reinterpret_cast() {
  int Base::*pb = &Base::base_field;

  // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: reinterpret_cast used to convert pointer-to-member from 'Base' to 'Unrelated'; invalid member pointer conversion
  int Unrelated::*pu = reinterpret_cast<int Unrelated::*>(pb);
  (void)pu;

  int Derived::*pd = &Derived::derived_field;

  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: reinterpret_cast used to convert pointer-to-member from 'Derived' to 'Base'; invalid member pointer conversion
  int Base::*pb2 = reinterpret_cast<int Base::*>(pd);
  (void)pb2;
}

// Test static_cast between unrelated classes (invalid)
void test_static_cast_unrelated() {
  int Base::*pb = &Base::base_field;

  // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: static_cast used to convert pointer-to-member from 'Base' to 'Unrelated'; conversion between unrelated classes is invalid
  int Unrelated::*pu = static_cast<int Unrelated::*>(pb);
  (void)pu;

  int Unrelated::*pu2 = &Unrelated::unrelated_field;

  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: static_cast used to convert pointer-to-member from 'Unrelated' to 'Base'; conversion between unrelated classes is invalid
  int Base::*pb2 = static_cast<int Base::*>(pu2);
  (void)pb2;
}

// Test valid conversions (derived to base with static_cast)
void test_valid_conversions() {
  // Derived to base is valid
  int Derived::*pd = &Derived::derived_field;
  int Base::*pb = static_cast<int Base::*>(pd);  // OK
  (void)pb;

  // Member function pointer - derived to base
  void (Derived::*pmd)() = &Derived::derived_method;
  void (Base::*pmb)() = static_cast<void (Base::*)()>(pmd);  // OK
  (void)pmb;

  // Same class conversion
  int Base::*pb1 = &Base::base_field;
  int Base::*pb2 = static_cast<int Base::*>(pb1);  // OK
  (void)pb2;
}

// Test member function pointers
void test_member_function_pointers() {
  void (Base::*pmb)() = &Base::base_method;

  // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: reinterpret_cast used to convert pointer-to-member from 'Base' to 'Unrelated'; invalid member pointer conversion
  void (Unrelated::*pmu)() = reinterpret_cast<void (Unrelated::*)()>(pmb);
  (void)pmu;

  void (Unrelated::*pmu2)() = &Unrelated::unrelated_method;

  // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: static_cast used to convert pointer-to-member from 'Unrelated' to 'Base'; conversion between unrelated classes is invalid
  void (Base::*pmb2)() = static_cast<void (Base::*)()>(pmu2);
  (void)pmb2;
}

// Test with const member functions
class ClassWithConst {
public:
  int field;
  void const_method() const {}
};

class UnrelatedConst {
public:
  int field;
  void const_method() const {}
};

void test_const_member_functions() {
  void (ClassWithConst::*pmc)() const = &ClassWithConst::const_method;

  // CHECK-MESSAGES: :[[@LINE+1]]:34: warning: static_cast used to convert pointer-to-member from 'ClassWithConst' to 'UnrelatedConst'; conversion between unrelated classes is invalid
  void (UnrelatedConst::*pmu)() const = static_cast<void (UnrelatedConst::*)() const>(pmc);
  (void)pmu;
}

// Test with multiple inheritance
class Base1 {
public:
  int base1_field;
};

class Base2 {
public:
  int base2_field;
};

class MultiDerived : public Base1, public Base2 {
public:
  int multi_field;
};

void test_multiple_inheritance() {
  // Derived to base1 is valid
  int MultiDerived::*pmd = &MultiDerived::multi_field;
  int Base1::*pb1 = static_cast<int Base1::*>(pmd);  // OK
  (void)pb1;

  // Derived to base2 is valid
  int Base2::*pb2 = static_cast<int Base2::*>(pmd);  // OK
  (void)pb2;

  // Base1 to Base2 conversion is invalid (they are unrelated in hierarchy)
  int Base1::*pb1_src = &Base1::base1_field;

  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: static_cast used to convert pointer-to-member from 'Base1' to 'Base2'; conversion between unrelated classes is invalid
  int Base2::*pb2_dst = static_cast<int Base2::*>(pb1_src);
  (void)pb2_dst;
}

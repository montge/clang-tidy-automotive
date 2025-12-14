// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-15.7 %t

// Test virtual destructor requirement

class BadBase1 {
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: class 'BadBase1' has virtual functions but no explicit virtual destructor [automotive-cpp23-req-15.7]
public:
  virtual void foo();
  ~BadBase1() {}
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: class 'BadBase1' has virtual functions but destructor is not virtual [automotive-cpp23-req-15.7]
};

class BadBase2 {
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: class 'BadBase2' has virtual functions but no explicit virtual destructor [automotive-cpp23-req-15.7]
public:
  virtual void bar() = 0;
  // No destructor declared - should warn
};

class GoodBase1 {
public:
  virtual void foo();
  virtual ~GoodBase1() {}
};

class GoodBase2 {
public:
  virtual void bar() = 0;
  virtual ~GoodBase2() = default;
};

class GoodDerived : public GoodBase1 {
public:
  // Destructor is virtual because base has virtual destructor
  ~GoodDerived() {}
};

class NoVirtualMethods {
public:
  void foo();
  ~NoVirtualMethods() {}
};

struct PlainStruct {
  int x;
  // No virtual methods - should not warn
};

class VirtualInherited : public GoodBase1 {
  // Inherits virtual destructor - should not warn
public:
  void foo() override;
};

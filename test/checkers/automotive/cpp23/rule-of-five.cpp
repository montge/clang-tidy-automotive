// RUN: %check_clang_tidy %s automotive-cpp23-req-15.0.1 %t
// Test for automotive-cpp23-req-15.0.1: Rule of Five

// Violation - defines destructor but not the others
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: class 'IncompleteResource' defines destructor but is missing copy constructor, copy assignment operator, move constructor and move assignment operator
class IncompleteResource {
  int* ptr;
public:
  IncompleteResource() : ptr(new int(0)) {}
  ~IncompleteResource() { delete ptr; }
};

// Violation - defines copy constructor and destructor
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: class 'PartialResource' defines destructor and copy constructor but is missing copy assignment operator, move constructor and move assignment operator
class PartialResource {
  int* ptr;
public:
  PartialResource() : ptr(new int(0)) {}
  PartialResource(const PartialResource& other) : ptr(new int(*other.ptr)) {}
  ~PartialResource() { delete ptr; }
};

// Compliant - defines all five
class CompleteResource {
  int* ptr;
public:
  CompleteResource() : ptr(new int(0)) {}
  ~CompleteResource() { delete ptr; }
  CompleteResource(const CompleteResource& other) : ptr(new int(*other.ptr)) {}
  CompleteResource& operator=(const CompleteResource& other) {
    if (this != &other) {
      delete ptr;
      ptr = new int(*other.ptr);
    }
    return *this;
  }
  CompleteResource(CompleteResource&& other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
  CompleteResource& operator=(CompleteResource&& other) noexcept {
    if (this != &other) {
      delete ptr;
      ptr = other.ptr;
      other.ptr = nullptr;
    }
    return *this;
  }
};

// Compliant - defines none (Rule of Zero)
class Simple {
  int value;
public:
  Simple() : value(0) {}
  int getValue() const { return value; }
};

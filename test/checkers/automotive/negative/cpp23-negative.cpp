// RUN: clang-tidy %s --checks='-*,automotive-cpp23-req-15.0.1,automotive-cpp23-req-8.2.1' -- -std=c++17 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant C++ code does not trigger warnings.

// Class following Rule of Five - compliant
class RuleOfFiveClass {
public:
  RuleOfFiveClass() = default;
  ~RuleOfFiveClass() = default;
  RuleOfFiveClass(const RuleOfFiveClass&) = default;
  RuleOfFiveClass& operator=(const RuleOfFiveClass&) = default;
  RuleOfFiveClass(RuleOfFiveClass&&) noexcept = default;
  RuleOfFiveClass& operator=(RuleOfFiveClass&&) noexcept = default;

  void doSomething() {}
};

// No special member functions defined - Rule of Zero - compliant
class SimpleClass {
public:
  void process(int x) {
    value = x;
  }
private:
  int value = 0;
};

// Explicit constructor - compliant
class ExplicitClass {
public:
  explicit ExplicitClass(int value) : m_value(value) {}
private:
  int m_value;
};

// Proper C++ style cast usage
void use_casts() {
  int x = 42;
  double d = static_cast<double>(x);  // C++ style cast
  [[maybe_unused]] auto result = d * 2.0;
}

// Using explicit constructor properly with C++ style
void use_explicit() {
  ExplicitClass obj(42);  // Explicit construction
  static_cast<void>(obj);  // C++ style void cast
}

// CHECK-NOT: warning:

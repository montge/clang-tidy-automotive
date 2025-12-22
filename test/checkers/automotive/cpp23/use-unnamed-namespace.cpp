// RUN: %check_clang_tidy %s automotive-cpp23-adv-10.3 %t

// MISRA C++:2023 Rule 10.3: An unnamed namespace should be used to define
// entities with internal linkage.

// Non-compliant: using static for internal linkage
static void staticFunc() {}
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: use an unnamed namespace instead of 'static' for internal linkage

static int staticVar = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use an unnamed namespace instead of 'static' for internal linkage

// Compliant: using unnamed namespace for internal linkage
namespace {
void unnamedFunc() {}
int unnamedVar = 42;
}

// Compliant: external linkage function
void externalFunc() {}

// Compliant: external linkage variable
int externalVar = 42;

class MyClass {
public:
  // Compliant: static member function (not file-scope static)
  static void staticMemberFunc() {}

  // Compliant: static data member
  static int staticMemberVar;
};

int MyClass::staticMemberVar = 0;

void someFunction() {
  // Compliant: local static variable (serves different purpose)
  static int localStatic = 0;
  localStatic++;
}

// Non-compliant: another static function
static int anotherStaticFunc(int x) { return x * 2; }
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use an unnamed namespace instead of 'static' for internal linkage

// Non-compliant: static const at file scope
static const int staticConst = 100;
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: use an unnamed namespace instead of 'static' for internal linkage

// Compliant: constexpr at file scope (has internal linkage by default in C++)
constexpr int constexprVar = 100;

// Compliant: const at file scope (has internal linkage by default in C++)
const int constVar = 100;

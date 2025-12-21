// RUN: %check_clang_tidy %s automotive-cpp23-req-18.4.2 %t -- -- -std=c++11

// Test: Move constructors and move assignment operators should be noexcept

class NoNoexcept {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: move constructor should be declared noexcept
  NoNoexcept(NoNoexcept&& other);

  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: move assignment operator should be declared noexcept
  NoNoexcept& operator=(NoNoexcept&& other);
};

class WithNoexcept {
public:
  // OK: marked noexcept
  WithNoexcept(WithNoexcept&& other) noexcept;

  // OK: marked noexcept
  WithNoexcept& operator=(WithNoexcept&& other) noexcept;
};

class WithNoexceptTrue {
public:
  // OK: noexcept(true) is acceptable
  WithNoexceptTrue(WithNoexceptTrue&& other) noexcept(true);

  // OK: noexcept(true) is acceptable
  WithNoexceptTrue& operator=(WithNoexceptTrue&& other) noexcept(true);
};

class WithNoexceptFalse {
public:
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: move constructor should be declared noexcept
  WithNoexceptFalse(WithNoexceptFalse&& other) noexcept(false);

  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: move assignment operator should be declared noexcept
  WithNoexceptFalse& operator=(WithNoexceptFalse&& other) noexcept(false);
};

class DeletedMove {
public:
  // OK: deleted move operations don't need noexcept
  DeletedMove(DeletedMove&& other) = delete;
  DeletedMove& operator=(DeletedMove&& other) = delete;
};

class CopyOnly {
public:
  // OK: not move operations
  CopyOnly(const CopyOnly& other);
  CopyOnly& operator=(const CopyOnly& other);
};

// Class with default move operations (implicitly declared)
// These should not trigger warnings as they are implicit
class DefaultMove {
public:
  int x;
  // Implicit move constructor and assignment are generated
};

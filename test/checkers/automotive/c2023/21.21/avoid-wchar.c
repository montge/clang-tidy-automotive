// RUN: %check_clang_tidy %s automotive-c23-req-21.21 %t -- -- -std=c99

// MISRA C:2025 Rule 21.21 (Required)
// The <wchar.h> and <wctype.h> headers shall not be used.

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inclusion of <wchar.h> is not allowed in safety-critical code [automotive-c23-req-21.21]
#include <wchar.h>

void example() {
  wchar_t buf[64];

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'wprintf' is not allowed in safety-critical code [automotive-c23-req-21.21]
  wprintf(L"Hello, world!\n");

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'wcscpy' is not allowed in safety-critical code [automotive-c23-req-21.21]
  wcscpy(buf, L"test");

  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of 'wcslen' is not allowed in safety-critical code [automotive-c23-req-21.21]
  size_t len = wcslen(buf);

  (void)len;
}

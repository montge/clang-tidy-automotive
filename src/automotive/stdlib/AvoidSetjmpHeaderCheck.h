//===--- AvoidSetjmpHeaderCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_SETJMPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_SETJMPCHECK_H

#include "AvoidApiCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of setjmp/longjmp functions from setjmp.h header.
///
/// The setjmp and longjmp functions provide non-local jumps that bypass
/// normal function call/return mechanisms. This can lead to undefined
/// behavior, resource leaks, and makes program flow difficult to analyze.
/// They are particularly problematic in safety-critical code.
///
/// Related MISRA C:2025 Rule: 21.4 - The standard header file <setjmp.h>
/// shall not be used.
///
/// Example:
/// \code
///   #include <setjmp.h>  // Warning: setjmp.h usage
///   jmp_buf env;
///   setjmp(env);         // Warning: setjmp usage
/// \endcode
class AvoidSetjmpHeaderCheck : public AvoidApiCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidSetjmpHeaderCheck(StringRef Name, ClangTidyContext *Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_SETJMPCHECK_H

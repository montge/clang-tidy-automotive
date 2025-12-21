//===--- AvoidCtypeHeaderCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCTYPEHEADERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCTYPEHEADERCHECK_H

#include "AvoidApiCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of <ctype.h> functions with potential issues.
///
/// MISRA C:2025 Rule 21.13 (Mandatory)
/// The values passed to ctype.h functions shall be representable as
/// an unsigned char or be the value EOF.
///
/// Example:
/// \code
///   #include <ctype.h>  // Warned
///   isdigit('a');       // Warned - potential issue if passed char < 0
/// \endcode
class AvoidCtypeHeaderCheck : public AvoidApiCheck {
public:
  AvoidCtypeHeaderCheck(StringRef Name, ClangTidyContext *Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCTYPEHEADERCHECK_H

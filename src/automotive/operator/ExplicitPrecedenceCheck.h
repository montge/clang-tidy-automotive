//===--- ExplicitPrecedenceCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXPLICITPRECEDENCECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXPLICITPRECEDENCECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects expressions where operator precedence may be confusing.
///
/// MISRA C:2012 Rule 12.1 (Advisory) requires that the precedence of
/// operators within expressions should be made explicit with parentheses.
///
/// This check flags common confusing patterns:
/// - Shift operators mixed with arithmetic (a + b << c)
/// - Bitwise operators mixed with comparison (a & b == c)
/// - Bitwise operators mixed with logical (a & b && c)
///
/// Example of non-compliant code:
/// \code
///   int x = a + b << c;    // + or << first?
///   int y = a & b == c;    // & or == first?
/// \endcode
///
/// Example of compliant code:
/// \code
///   int x = (a + b) << c;  // Explicit precedence
///   int y = a & (b == c);  // Explicit precedence
/// \endcode
class ExplicitPrecedenceCheck : public ClangTidyCheck {
public:
  ExplicitPrecedenceCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~ExplicitPrecedenceCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isConfusingPrecedence(BinaryOperatorKind Parent,
                             BinaryOperatorKind Child) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXPLICITPRECEDENCECHECK_H

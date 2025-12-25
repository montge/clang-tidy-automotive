//===--- AvoidOverlappingCopyCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOVERLAPPINGCOPYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOVERLAPPINGCOPYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects copy/move operations where source and destination may overlap.
///
/// MISRA C++:2023 Rule 13.1: Copy/move operations should not have overlapping
/// source and destination.
///
/// Overlapping copy operations can lead to undefined or unexpected behavior.
/// This includes self-assignment, calls to memcpy with overlapping ranges,
/// and copy/move constructors called with the same source and destination.
///
/// Example violations:
/// @code
///   struct S {
///     S& operator=(const S& other) {
///       if (this != &other) {  // Self-assignment check
///         // ...
///       }
///       return *this;
///     }
///   };
///
///   void foo() {
///     S obj;
///     obj = obj;  // Warning: self-assignment
///
///     char buf[10];
///     memcpy(buf + 2, buf, 5);  // Warning: potentially overlapping ranges
///   }
/// @endcode
///
/// Compliant:
/// @code
///   void foo() {
///     S obj1, obj2;
///     obj1 = obj2;  // OK - different objects
///
///     char buf[10];
///     memmove(buf + 2, buf, 5);  // OK - memmove handles overlap
///   }
/// @endcode
class AvoidOverlappingCopyCheck : public ClangTidyCheck {
public:
  AvoidOverlappingCopyCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

private:
  /// Check if two expressions refer to the same object
  bool areSameObject(const Expr *E1, const Expr *E2,
                     const ASTContext &Ctx) const;

  /// Check if memory ranges may overlap
  bool mayOverlap(const Expr *Dest, const Expr *Src, const Expr *Size,
                  const ASTContext &Ctx) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOVERLAPPINGCOPYCHECK_H

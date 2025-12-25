//===--- AvoidWideVariableScopeCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDWIDEVARIABLESCOPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDWIDEVARIABLESCOPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-declarations
/// @brief Detects variables declared in unnecessarily wide scopes.
///
/// Variables should be declared in the smallest scope possible, ideally
/// close to their first use. This improves code readability and reduces
/// the potential for errors.
///
/// @par MISRA C++:2023 Rule 5.7 (Advisory)
/// Variables should be declared in the smallest scope possible.
///
/// Example:
/// @code
///   void foo() {
///       int x = 10;  // Non-compliant if only used in inner block
///
///       if (condition) {
///           use(x);  // x only used here
///       }
///   }
///
///   // Compliant:
///   void bar() {
///       if (condition) {
///           int x = 10;  // Declared in minimal scope
///           use(x);
///       }
///   }
/// @endcode
class AvoidWideVariableScopeCheck : public ClangTidyCheck {
public:
  AvoidWideVariableScopeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Finds the deepest compound statement that contains all uses of a variable
  const CompoundStmt *findMinimalScope(const VarDecl *VD,
                                       const CompoundStmt *DeclScope,
                                       ASTContext *Context) const;

  /// Collects all DeclRefExpr nodes that reference the given variable
  void collectVarReferences(const Stmt *Root, const VarDecl *VD,
                            SmallVectorImpl<const DeclRefExpr *> &Refs) const;

  /// Checks if a statement is an ancestor of another statement
  bool isAncestor(const Stmt *Ancestor, const Stmt *Descendant,
                  ASTContext *Context) const;

  /// Finds the innermost compound statement that contains a given statement
  const CompoundStmt *findContainingCompoundStmt(const Stmt *S,
                                                 const Stmt *Root) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDWIDEVARIABLESCOPECHECK_H

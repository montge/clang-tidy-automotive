//===--- AvoidCommentWithinCommentCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMENTWITHINCOMMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMENTWITHINCOMMENTCHECK_H

#include "../../ClangTidyCheck.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringSet.h"

namespace clang::tidy::automotive {

/// Detects nested comment sequences within C-style comments.
///
/// Attempting to nest comments can lead to unintended code being commented out
/// or activated, as the first occurrence of */ will close the outer comment.
/// This can cause logic errors and make code difficult to maintain.
///
/// Related MISRA C:2025 Rule: 3.1 - The character sequences /* and // shall
/// not appear within a comment.
///
/// Example:
/// \code
///   /* This is a comment /* with nested comment */ markers */  // Warning
/// \endcode
class AvoidCommentWithinCommentCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  /// \param x Unused parameter for compatibility.
  AvoidCommentWithinCommentCheck(StringRef Name, ClangTidyContext *Context,
                                 int x = 0)
      : ClangTidyCheck(Name, Context), Handler(*this) {}

  /// Registers preprocessor callbacks for comment handling.
  /// \param SM The source manager.
  /// \param PP The preprocessor instance.
  /// \param ModuleExpanderPP The module expander preprocessor.
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

private:
  class InternalCommentHandler : public CommentHandler {
  public:
    InternalCommentHandler(AvoidCommentWithinCommentCheck &Check);
    virtual bool HandleComment(Preprocessor &PP, SourceRange Comment) override;

  private:
    void CheckComment(SourceLocation CommentLoc, StringRef CommentText);

    AvoidCommentWithinCommentCheck &Check;
    llvm::StringSet<> Protocols;
  };

  InternalCommentHandler Handler;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMENTWITHINCOMMENTCHECK_H

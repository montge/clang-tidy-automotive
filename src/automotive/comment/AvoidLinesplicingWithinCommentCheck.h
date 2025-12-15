//===--- AvoidLinesplicingWithinCommentCheck.h - clang-tidy -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLINESPLICINGWITHINCOMMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLINESPLICINGWITHINCOMMENTCHECK_H

#include "../../ClangTidyCheck.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::automotive {

/// Detects line splicing (backslash-newline) within comments.
///
/// Line splicing inside comments can cause confusion about comment boundaries
/// and may lead to unexpected behavior where code intended to be active gets
/// commented out. This is particularly dangerous with // comments where a
/// trailing backslash continues the comment to the next line.
///
/// Related MISRA C:2025 Rule: 3.2 - Line-splicing shall not be used in
/// // comments.
///
/// Example:
/// \code
///   // This comment has line splicing \
///      and continues here  // Warning
/// \endcode
class AvoidLinesplicingWithinCommentCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidLinesplicingWithinCommentCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        Handler(*this, Options.get("FixitEnabled", false)) {}

  /// Destructor.
  ~AvoidLinesplicingWithinCommentCheck() override = default;

  /// Registers preprocessor callbacks for comment handling.
  /// \param SM The source manager.
  /// \param PP The preprocessor instance.
  /// \param ModuleExpanderPP The module expander preprocessor.
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

private:
  class InternalCommentHandler : public CommentHandler {
  public:
    InternalCommentHandler(ClangTidyCheck &Check, bool FixitEnabled)
        : Check(Check), FixitEnabled(FixitEnabled) {}
    ~InternalCommentHandler() override = default;
    bool HandleComment(Preprocessor &PP, SourceRange Comment) override;

  private:
    ClangTidyCheck &Check;
    bool FixitEnabled;
  };

  InternalCommentHandler Handler;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLINESPLICINGWITHINCOMMENTCHECK_H

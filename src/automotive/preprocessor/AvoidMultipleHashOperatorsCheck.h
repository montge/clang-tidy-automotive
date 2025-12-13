//===--- AvoidMultipleHashOperatorsCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMULTIPLEHASHOPERATORSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMULTIPLEHASHOPERATORSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of the ## token pasting operator in macros.
///
/// The ## operator in preprocessor macros concatenates tokens, which can
/// create identifiers in unexpected ways. This makes code harder to search,
/// understand, and debug. Token pasting can also lead to subtle bugs when
/// tokens combine in unintended ways.
///
/// Related MISRA C:2025 Rule: 20.11 - The ## operator should not be used.
///
/// Example:
/// \code
///   #define CONCAT(a, b) a##b  // Warning: ## operator used
/// \endcode
class AvoidMultipleHashOperatorsCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidMultipleHashOperatorsCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers preprocessor callbacks for macro definition handling.
  /// \param SM The source manager.
  /// \param PP The preprocessor instance.
  /// \param ModuleExpanderPP The module expander preprocessor.
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMULTIPLEHASHOPERATORSCHECK_H

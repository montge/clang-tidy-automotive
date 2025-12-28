//===--- AvoidApiCheck.h - clang-tidy --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAPICHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAPICHECK_H

#include "../../ClangTidyCheck.h"
#include "AvoidApiHandler.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"

namespace clang::tidy::automotive {

/// Base class for checks that detect usage of specific standard library APIs.
///
/// This check provides a framework for detecting and warning about the use
/// of specific standard library functions and headers that may be prohibited
/// in safety-critical code. Derived checks specify which functions and headers
/// to flag.
///
/// Related MISRA C:2025 Rule: Various rules related to standard library usage.
///
/// Example:
/// \code
///   // Derived checks will flag specific function calls
/// \endcode
class AvoidApiCheck : public ClangTidyCheck {
public:
  /// Registers preprocessor callbacks for header inclusion checking.
  /// \param SM The source manager.
  /// \param PP The preprocessor instance.
  /// \param ModuleExpanderPP The module expander preprocessor.
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

  /// Registers AST matchers for function calls.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched function calls and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

protected:
  /// Constructs the check with header and function names.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  /// \param Header The header file to check for.
  /// \param FunctionNames List of function names to flag.
  AvoidApiCheck(StringRef Name, ClangTidyContext *Context, StringRef Header,
                const std::initializer_list<llvm::StringRef> &FunctionNames)
      : ClangTidyCheck(Name, Context), FunctionNames(FunctionNames),
        HeaderName(Header) {}

  /// Destructor.
  ~AvoidApiCheck() override = default;

  /// Constructs the check with function names only.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  /// \param FunctionNames List of function names to flag.
  AvoidApiCheck(StringRef Name, ClangTidyContext *Context,
                const std::initializer_list<llvm::StringRef> &FunctionNames)
      : ClangTidyCheck(Name, Context), FunctionNames(FunctionNames) {}

  /// Constructs the check with header and function name array.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  /// \param HeaderName The header file to check for.
  /// \param FunctionNames Array of function names to flag.
  AvoidApiCheck(StringRef Name, ClangTidyContext *Context, StringRef HeaderName,
                ArrayRef<StringRef> FunctionNames)
      : ClangTidyCheck(Name, Context), FunctionNames(FunctionNames),
        HeaderName(HeaderName) {}

private:
  ArrayRef<StringRef> FunctionNames;
  StringRef HeaderName;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAPICHECK_H

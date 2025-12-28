//===--- GenericMacroNestingCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICMACRONESTINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICMACRONESTINGCHECK_H

#include "../../ClangTidyCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <map>
#include <set>
#include <string>

namespace clang::tidy::automotive {

/// Detects function-like macros that invoke other macros with generic type
/// parameters.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. When creating function-like macros that use generic
/// selections, it's important to avoid nesting macro calls where generic type
/// parameters are passed between macros.
///
/// This check warns when a function-like macro containing a generic selection
/// calls another function-like macro and passes a parameter that is used in
/// the generic selection's controlling expression or associations. This can
/// lead to:
/// - Unexpected type resolution
/// - Multiple evaluations of parameters
/// - Difficult-to-debug macro expansion issues
///
/// The check uses preprocessor callbacks to track macro definitions and
/// expansions, identifying problematic nesting patterns.
///
/// Related MISRA C:2025 Rule: 23.6 - A function-like macro shall not invoke
/// another function-like macro with generic type parameter (Required).
///
/// Example:
/// \code
///   #define TYPE_SELECT(x) _Generic((x), int: 1, float: 2)
///
///   // Non-compliant: passes parameter to another macro with _Generic
///   #define WRAPPER(x) TYPE_SELECT(x)
///
///   // Compliant: inline generic selection
///   #define DIRECT(x) _Generic((x), int: 1, float: 2)
///
///   // Non-compliant: nested macro call with generic parameter
///   #define INNER(x) _Generic((x), int: process_int(x), float: process_float(x))
///   #define OUTER(y) INNER(y)
/// \endcode
class GenericMacroNestingCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericMacroNestingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericMacroNestingCheck() override = default;

  /// Registers preprocessor callbacks.
  /// \param SM The source manager.
  /// \param PP The preprocessor.
  /// \param ModuleExpanderPP The module expander preprocessor (unused).
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Stores information about macros containing generic selections.
  std::set<std::string> MacrosWithGeneric;

  /// Stores macro expansion locations for analysis.
  std::map<SourceLocation, std::string> MacroExpansions;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICMACRONESTINGCHECK_H

//===--- TgmathOperandTypeCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TgmathOperandTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringSet.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Type-generic macros from <tgmath.h> (C99/C11)
// These macros expand to appropriate math.h or complex.h functions
// based on the argument types
const llvm::StringSet<> TgmathMacros = {
    // Trigonometric functions
    "acos",
    "asin",
    "atan",
    "atan2",
    "cos",
    "sin",
    "tan",
    // Hyperbolic functions
    "acosh",
    "asinh",
    "atanh",
    "cosh",
    "sinh",
    "tanh",
    // Exponential and logarithmic functions
    "exp",
    "exp2",
    "expm1",
    "log",
    "log10",
    "log1p",
    "log2",
    "logb",
    // Power functions
    "pow",
    "sqrt",
    "cbrt",
    "hypot",
    // Error and gamma functions
    "erf",
    "erfc",
    "tgamma",
    "lgamma",
    // Rounding functions
    "ceil",
    "floor",
    "trunc",
    "round",
    "lround",
    "llround",
    "nearbyint",
    "rint",
    "lrint",
    "llrint",
    // Floating-point manipulation
    "frexp",
    "ldexp",
    "modf",
    "scalbn",
    "scalbln",
    "ilogb",
    "nextafter",
    "nexttoward",
    "copysign",
    // Absolute value and remainder
    "fabs",
    "fmod",
    "remainder",
    "remquo",
    "fma",
    "fmax",
    "fmin",
    "fdim",
    // Complex functions
    "carg",
    "cimag",
    "conj",
    "cproj",
    "creal",
};

/// Track tgmath.h inclusion
class TgmathOperandTypePPCallbacks : public PPCallbacks {
public:
  TgmathOperandTypePPCallbacks(TgmathOperandTypeCheck & /*Check*/,
                               const SourceManager & /*SM*/) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {
    (void)HashLoc;
    (void)IncludeTok;
    (void)IsAngled;
    (void)FilenameRange;
    (void)File;
    (void)SearchPath;
    (void)RelativePath;
    (void)SuggestedModule;
    (void)ModuleImported;
    (void)FileType;
    if (FileName == "tgmath.h") {
      TgmathIncluded = true;
    }
  }

  bool isTgmathIncluded() const { return TgmathIncluded; }

private:
  bool TgmathIncluded = false;
};

} // anonymous namespace

bool TgmathOperandTypeCheck::isTgmathMacro(StringRef Name) const {
  return TgmathMacros.contains(Name);
}

void TgmathOperandTypeCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<TgmathOperandTypePPCallbacks>(*this, SM));
}

void TgmathOperandTypeCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to functions that could be tgmath.h macros
  // We need to check if they're actually macro expansions and if the
  // arguments have appropriate types
  Finder->addMatcher(
      callExpr(
          unless(isExpansionInSystemHeader()),
          callee(functionDecl(hasAnyName(
              // Trigonometric functions
              "acos", "acosf", "acosl", "asin", "asinf", "asinl", "atan",
              "atanf", "atanl", "atan2", "atan2f", "atan2l", "cos", "cosf",
              "cosl", "sin", "sinf", "sinl", "tan", "tanf", "tanl",
              // Hyperbolic functions
              "acosh", "acoshf", "acoshl", "asinh", "asinhf", "asinhl", "atanh",
              "atanhf", "atanhl", "cosh", "coshf", "coshl", "sinh", "sinhf",
              "sinhl", "tanh", "tanhf", "tanhl",
              // Exponential and logarithmic functions
              "exp", "expf", "expl", "exp2", "exp2f", "exp2l", "expm1",
              "expm1f", "expm1l", "log", "logf", "logl", "log10", "log10f",
              "log10l", "log1p", "log1pf", "log1pl", "log2", "log2f", "log2l",
              "logb", "logbf", "logbl",
              // Power functions
              "pow", "powf", "powl", "sqrt", "sqrtf", "sqrtl", "cbrt", "cbrtf",
              "cbrtl", "hypot", "hypotf", "hypotl",
              // Error and gamma functions
              "erf", "erff", "erfl", "erfc", "erfcf", "erfcl", "tgamma",
              "tgammaf", "tgammal", "lgamma", "lgammaf", "lgammal",
              // Rounding functions
              "ceil", "ceilf", "ceill", "floor", "floorf", "floorl", "trunc",
              "truncf", "truncl", "round", "roundf", "roundl", "lround",
              "lroundf", "lroundl", "llround", "llroundf", "llroundl",
              "nearbyint", "nearbyintf", "nearbyintl", "rint", "rintf", "rintl",
              "lrint", "lrintf", "lrintl", "llrint", "llrintf", "llrintl",
              // Floating-point manipulation
              "frexp", "frexpf", "frexpl", "ldexp", "ldexpf", "ldexpl", "modf",
              "modff", "modfl", "scalbn", "scalbnf", "scalbnl", "scalbln",
              "scalblnf", "scalblnl", "ilogb", "ilogbf", "ilogbl", "nextafter",
              "nextafterf", "nextafterl", "nexttoward", "nexttowardf",
              "nexttowardl", "copysign", "copysignf", "copysignl",
              // Absolute value and remainder
              "fabs", "fabsf", "fabsl", "fmod", "fmodf", "fmodl", "remainder",
              "remainderf", "remainderl", "remquo", "remquof", "remquol", "fma",
              "fmaf", "fmal", "fmax", "fmaxf", "fmaxl", "fmin", "fminf",
              "fminl", "fdim", "fdimf", "fdiml"))))
          .bind("tgmathCall"),
      this);
}

void TgmathOperandTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("tgmathCall");
  if (!Call)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  // Get the function name to determine the base macro name
  const FunctionDecl *FD = Call->getDirectCallee();
  if (!FD)
    return;

  StringRef FuncName = FD->getName();

  // Extract the base name (remove f/l suffix for float/long double variants)
  std::string BaseName = FuncName.str();
  if (!BaseName.empty() && (BaseName.back() == 'f' || BaseName.back() == 'l')) {
    BaseName = BaseName.substr(0, BaseName.length() - 1);
  }

  // Check if this is a tgmath macro
  if (!isTgmathMacro(BaseName))
    return;

  // Check if the call is from a macro expansion
  SourceLocation CallLoc = Call->getBeginLoc();
  if (!CallLoc.isMacroID())
    return; // Not a macro expansion, direct call is OK

  // Get the immediate macro name
  StringRef MacroName = Lexer::getImmediateMacroName(
      CallLoc, *Result.SourceManager, Result.Context->getLangOpts());

  // Check if the macro name matches a tgmath macro
  if (!isTgmathMacro(MacroName))
    return;

  // Now check the argument types
  // For type-generic macros, all arguments should be floating-point or complex
  for (unsigned I = 0; I < Call->getNumArgs(); ++I) {
    const Expr *Arg = Call->getArg(I);
    if (!Arg)
      continue;

    QualType ArgType = Arg->getType();

    // Skip pointer arguments (used by some functions like frexp, modf)
    if (ArgType->isPointerType())
      continue;

    // Skip if the type is dependent (template context)
    if (ArgType->isDependentType())
      continue;

    // Check if the argument is an appropriate type
    // Appropriate types: float, double, long double, complex types
    if (ArgType->isRealFloatingType()) {
      // float, double, long double are OK
      continue;
    }
    if (ArgType->isComplexType()) {
      // _Complex types are OK
      continue;
    }
    if (ArgType->isIntegerType()) {
      // Integer types are NOT appropriate
      // Get the spelling location for better error reporting
      SourceLocation SpellingLoc =
          Result.SourceManager->getSpellingLoc(CallLoc);

      diag(SpellingLoc,
           "argument %0 to type-generic macro '%1' has inappropriate type "
           "'%2'; expected floating-point or complex type")
          << (I + 1) << MacroName << ArgType;
    }
    // Other types (e.g., enum, struct) are also inappropriate
    // But we focus on integer types as the primary violation
  }
}

} // namespace clang::tidy::automotive

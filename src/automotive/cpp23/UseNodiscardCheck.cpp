//===--- UseNodiscardCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseNodiscardCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

UseNodiscardCheck::UseNodiscardCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      CheckPrivateFunctions(Options.get("CheckPrivateFunctions", false)) {}

void UseNodiscardCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "CheckPrivateFunctions", CheckPrivateFunctions);
}

bool UseNodiscardCheck::shouldCheckReturnType(QualType ReturnType) const {
  if (ReturnType.isNull() || ReturnType->isVoidType())
    return false;

  // Remove qualifiers and references to get the underlying type
  QualType BaseType = ReturnType.getNonReferenceType().getUnqualifiedType();

  // Check for bool - commonly used for error indicators
  if (BaseType->isBooleanType())
    return true;

  // Check for pointers - nullptr indicates failure
  if (BaseType->isPointerType())
    return true;

  // Check for integral types that might be error codes
  if (BaseType->isIntegerType()) {
    // Check if it's an enum (error codes are often enums)
    if (BaseType->isEnumeralType())
      return true;

    // Also flag plain int/unsigned/etc that might be error codes
    // but exclude char types (less likely to be error codes)
    if (!BaseType->isAnyCharacterType())
      return true;
  }

  // Check for optional-like types: std::optional, std::expected
  if (const auto *RT = BaseType->getAs<RecordType>()) {
    const auto *RD = RT->getDecl();
    if (const auto *CTSD = dyn_cast<ClassTemplateSpecializationDecl>(RD)) {
      StringRef Name = CTSD->getName();
      if (Name == "optional" || Name == "expected" || Name == "unique_ptr" ||
          Name == "shared_ptr")
        return true;
    }
  }

  return false;
}

void UseNodiscardCheck::registerMatchers(MatchFinder *Finder) {
  // Match function declarations that return non-void
  Finder->addMatcher(
      functionDecl(
          returns(unless(voidType())), unless(isImplicit()),
          unless(isDefaulted()), unless(isDeleted()),
          unless(cxxConstructorDecl()), unless(cxxDestructorDecl()),
          unless(cxxConversionDecl()),
          unless(hasAttr(attr::WarnUnusedResult)), // [[nodiscard]]
          unless(hasAncestor(
              functionTemplateDecl())) // Skip template function declarations,
                                       // match specializations instead
          )
          .bind("func"),
      this);
}

void UseNodiscardCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!Func)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Func->getLocation()))
    return;

  // Skip operators (assignments, conversions, etc.)
  if (isa<CXXMethodDecl>(Func)) {
    const auto *Method = cast<CXXMethodDecl>(Func);
    if (Method->isOverloadedOperator())
      return;

    // Check access level for member functions
    if (!CheckPrivateFunctions) {
      AccessSpecifier Access = Method->getAccess();
      if (Access == AS_private || Access == AS_protected)
        return;
    }
  }

  // Check if return type should be checked
  QualType ReturnType = Func->getReturnType();
  if (!shouldCheckReturnType(ReturnType))
    return;

  // Skip if this is a function definition without a separate declaration
  // and it's static (internal linkage)
  if (const auto *FD = dyn_cast<FunctionDecl>(Func)) {
    if (FD->getStorageClass() == SC_Static && !FD->isFirstDecl())
      return;
  }

  // Emit diagnostic
  StringRef TypeCategory;
  QualType BaseType = ReturnType.getNonReferenceType().getUnqualifiedType();

  if (BaseType->isBooleanType())
    TypeCategory = "boolean (potential error indicator)";
  else if (BaseType->isPointerType())
    TypeCategory = "pointer (potential null on failure)";
  else if (BaseType->isEnumeralType())
    TypeCategory = "enumeration (potential error code)";
  else if (BaseType->isIntegerType())
    TypeCategory = "integer (potential error code)";
  else
    TypeCategory = "value that should not be ignored";

  diag(Func->getLocation(),
       "function returns %0 but is not marked [[nodiscard]]; functions that "
       "return values that should not be ignored shall be declared with the "
       "[[nodiscard]] attribute (MISRA C++:2023 Rule 10.6.1)")
      << TypeCategory;
}

} // namespace clang::tidy::automotive::cpp23

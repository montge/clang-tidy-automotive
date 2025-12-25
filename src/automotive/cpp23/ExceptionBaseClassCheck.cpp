//===--- ExceptionBaseClassCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExceptionBaseClassCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
// Helper function to check if a type derives from std::exception
bool isDerivedFromStdException(const QualType &QT, ASTContext &Ctx) {
  // Get the canonical type without qualifiers
  const clang::Type *CanonicalType = QT.getCanonicalType().getTypePtr();

  // If it's a pointer or reference, get the pointee type
  if (const auto *PtrType = CanonicalType->getAs<PointerType>())
    CanonicalType = PtrType->getPointeeType().getCanonicalType().getTypePtr();
  else if (const auto *RefType = CanonicalType->getAs<ReferenceType>())
    CanonicalType = RefType->getPointeeType().getCanonicalType().getTypePtr();

  // Must be a class type
  const auto *RT = CanonicalType->getAs<clang::RecordType>();
  if (!RT)
    return false;

  const auto *RD = RT->getDecl();
  const auto *CXXRD = dyn_cast<clang::CXXRecordDecl>(RD);
  if (!CXXRD)
    return false;

  // Check if this is std::exception itself
  if (CXXRD->getQualifiedNameAsString() == "std::exception")
    return true;

  // Check all base classes
  if (!CXXRD->hasDefinition())
    return false;

  for (const auto &Base : CXXRD->bases()) {
    QualType BaseType = Base.getType();
    if (isDerivedFromStdException(BaseType, Ctx))
      return true;
  }

  return false;
}
} // namespace

void ExceptionBaseClassCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match throw expressions that throw something (not re-throw)
  // A re-throw is a throw expression without an operand (throw;)
  Finder->addMatcher(cxxThrowExpr(has(expr().bind("thrown"))).bind("throw"),
                     this);
}

void ExceptionBaseClassCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Throw = Result.Nodes.getNodeAs<CXXThrowExpr>("throw");
  const auto *Thrown = Result.Nodes.getNodeAs<Expr>("thrown");

  // LCOV_EXCL_START - defensive check, matcher guarantees these exist
  if (!Throw || !Thrown)
    return;
  // LCOV_EXCL_STOP

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Throw->getThrowLoc()))
    return;

  // Get the type being thrown
  QualType ThrownType = Thrown->getType();

  // Check if the type derives from std::exception
  if (isDerivedFromStdException(ThrownType, *Result.Context))
    return;

  // If we get here, it's a violation
  const clang::Type *CanonicalType = ThrownType.getCanonicalType().getTypePtr();

  // Provide a more specific message based on the type
  if (CanonicalType->isArrayType() ||
      (CanonicalType->isPointerType() &&
       isa<StringLiteral>(Thrown->IgnoreParenImpCasts()))) {
    // String literal (array of char or const char*)
    diag(Throw->getThrowLoc(),
         "throwing C-string literal is not allowed; exceptions must derive "
         "from std::exception (MISRA C++:2023 Rule 18.3.1)");
  } else if (CanonicalType->isPointerType()) {
    diag(Throw->getThrowLoc(),
         "throwing pointer type %0 is not allowed; exceptions must derive "
         "from std::exception (MISRA C++:2023 Rule 18.3.1)")
        << ThrownType;
  } else if (CanonicalType->isBuiltinType()) {
    diag(Throw->getThrowLoc(),
         "throwing primitive type %0 is not allowed; exceptions must derive "
         "from std::exception (MISRA C++:2023 Rule 18.3.1)")
        << ThrownType;
  } else if (const auto *RT = CanonicalType->getAs<clang::RecordType>()) {
    diag(Throw->getThrowLoc(),
         "throwing class %0 that does not derive from std::exception is not "
         "allowed (MISRA C++:2023 Rule 18.3.1)")
        << RT->getDecl();
  } else {
    diag(Throw->getThrowLoc(),
         "throwing type %0 that does not derive from std::exception is not "
         "allowed (MISRA C++:2023 Rule 18.3.1)")
        << ThrownType;
  }
}

} // namespace clang::tidy::automotive

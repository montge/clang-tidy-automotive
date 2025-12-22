//===--- AvoidCharacterTypeMismatchCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCharacterTypeMismatchCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

bool AvoidCharacterTypeMismatchCheck::isCharacterType(
    const QualType &QT) const {
  const Type *T = QT.getCanonicalType().getTypePtr();
  if (const auto *BT = dyn_cast<BuiltinType>(T)) {
    switch (BT->getKind()) {
    case BuiltinType::Char_U:
    case BuiltinType::Char_S:
    case BuiltinType::SChar:
    case BuiltinType::UChar:
    case BuiltinType::WChar_U:
    case BuiltinType::WChar_S:
    case BuiltinType::Char8:
    case BuiltinType::Char16:
    case BuiltinType::Char32:
      return true;
    default:
      return false;
    }
  }
  return false;
}

StringRef
AvoidCharacterTypeMismatchCheck::getCharTypeName(const QualType &QT) const {
  const Type *T = QT.getCanonicalType().getTypePtr();
  if (const auto *BT = dyn_cast<BuiltinType>(T)) {
    switch (BT->getKind()) {
    case BuiltinType::Char_U:
    case BuiltinType::Char_S:
      return "char";
    case BuiltinType::SChar:
      return "signed char";
    case BuiltinType::UChar:
      return "unsigned char";
    case BuiltinType::WChar_U:
    case BuiltinType::WChar_S:
      return "wchar_t";
    case BuiltinType::Char8:
      return "char8_t";
    case BuiltinType::Char16:
      return "char16_t";
    case BuiltinType::Char32:
      return "char32_t";
    default:
      return "unknown";
    }
  }
  return "unknown";
}

void AvoidCharacterTypeMismatchCheck::registerMatchers(MatchFinder *Finder) {
  // Match assignments where LHS and RHS are character types
  Finder->addMatcher(binaryOperator(isAssignmentOperator(),
                                    hasLHS(expr().bind("lhs")),
                                    hasRHS(expr().bind("rhs")))
                         .bind("assign"),
                     this);

  // Match variable declarations with initializers
  Finder->addMatcher(varDecl(hasInitializer(expr().bind("init"))).bind("var"),
                     this);
}

void AvoidCharacterTypeMismatchCheck::check(
    const MatchFinder::MatchResult &Result) {

  if (const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
    const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

    if (!LHS || !RHS)
      return;

    QualType LHSType = LHS->getType();
    // Strip implicit casts to get the original type
    const Expr *RHSStripped = RHS->IgnoreParenImpCasts();
    QualType RHSType = RHSStripped->getType();

    // Skip if not both character types
    if (!isCharacterType(LHSType) || !isCharacterType(RHSType))
      return;

    // Get canonical types for comparison
    QualType LHSCanon = LHSType.getCanonicalType();
    QualType RHSCanon = RHSType.getCanonicalType();

    // Check if types are different
    if (LHSCanon != RHSCanon) {
      diag(Assign->getOperatorLoc(),
           "assignment between different character types: '%0' and '%1'")
          << getCharTypeName(LHSType) << getCharTypeName(RHSType);
    }
  }

  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
    const auto *Init = Result.Nodes.getNodeAs<Expr>("init");

    if (!Init)
      return;

    QualType VarType = Var->getType();
    // Strip implicit casts to get the original type
    const Expr *InitStripped = Init->IgnoreParenImpCasts();
    QualType InitType = InitStripped->getType();

    // Skip if not both character types
    if (!isCharacterType(VarType) || !isCharacterType(InitType))
      return;

    // Get canonical types for comparison
    QualType VarCanon = VarType.getCanonicalType();
    QualType InitCanon = InitType.getCanonicalType();

    // Check if types are different
    if (VarCanon != InitCanon) {
      diag(Var->getLocation(), "initialization with different character type: "
                               "variable is '%0' but initializer is '%1'")
          << getCharTypeName(VarType) << getCharTypeName(InitType);
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

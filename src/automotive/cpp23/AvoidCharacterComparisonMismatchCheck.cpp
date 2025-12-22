//===--- AvoidCharacterComparisonMismatchCheck.cpp - clang-tidy -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCharacterComparisonMismatchCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

bool AvoidCharacterComparisonMismatchCheck::isCharacterType(
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

StringRef AvoidCharacterComparisonMismatchCheck::getCharTypeName(
    const QualType &QT) const {
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

void AvoidCharacterComparisonMismatchCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match comparison operators with character operands
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("=="), hasOperatorName("!="),
                           hasOperatorName("<"), hasOperatorName(">"),
                           hasOperatorName("<="), hasOperatorName(">=")),
                     hasLHS(expr().bind("lhs")), hasRHS(expr().bind("rhs")))
          .bind("compare"),
      this);
}

void AvoidCharacterComparisonMismatchCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Compare = Result.Nodes.getNodeAs<BinaryOperator>("compare");
  const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
  const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!Compare || !LHS || !RHS)
    return;

  // Strip implicit casts to get original types
  const Expr *LHSStripped = LHS->IgnoreParenImpCasts();
  const Expr *RHSStripped = RHS->IgnoreParenImpCasts();

  QualType LHSType = LHSStripped->getType();
  QualType RHSType = RHSStripped->getType();

  // Only check if at least one operand is a character type
  bool LHSIsChar = isCharacterType(LHSType);
  bool RHSIsChar = isCharacterType(RHSType);

  if (!LHSIsChar && !RHSIsChar)
    return;

  // If both are character types, check they're the same
  if (LHSIsChar && RHSIsChar) {
    QualType LHSCanon = LHSType.getCanonicalType();
    QualType RHSCanon = RHSType.getCanonicalType();

    if (LHSCanon != RHSCanon) {
      diag(Compare->getOperatorLoc(),
           "comparison between different character types: '%0' and '%1'")
          << getCharTypeName(LHSType) << getCharTypeName(RHSType);
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

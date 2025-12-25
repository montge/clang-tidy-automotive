//===--- AvoidTypeidOnDereferencedPointerCheck.cpp - clang-tidy ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTypeidOnDereferencedPointerCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

// Define destructor (key function for vtable)
AvoidTypeidOnDereferencedPointerCheck::
    ~AvoidTypeidOnDereferencedPointerCheck() = default;

namespace {
// Matcher for CXXTypeidExpr nodes - this is the canonical way to create a
// node matcher in LLVM for types that don't have a built-in matcher.
const internal::VariadicDynCastAllOfMatcher<Stmt, CXXTypeidExpr> cxxTypeidExpr;
} // namespace

void AvoidTypeidOnDereferencedPointerCheck::registerMatchers(
    MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match typeid expressions with an expression argument (not a type)
  // We need to check if the operand involves dereferencing a pointer
  // to a polymorphic type.
  Finder->addMatcher(cxxTypeidExpr(unless(isTypeDependent())).bind("typeid"),
                     this);
}

/// Check if the expression is a dereference of a pointer (unary * or ->)
static bool isDereferenceOfPointer(const Expr *E) {
  if (!E)
    return false;

  // Strip implicit casts and parentheses
  E = E->IgnoreParenImpCasts();

  // Check for unary dereference operator (operator*)
  if (const auto *UO = dyn_cast<UnaryOperator>(E)) {
    if (UO->getOpcode() == UO_Deref) {
      // Check if the subexpression is a pointer type
      return UO->getSubExpr()->getType()->isPointerType();
    }
  }

  // Check for member access through pointer (operator->)
  if (const auto *ME = dyn_cast<MemberExpr>(E)) {
    return ME->isArrow(); // isArrow() returns true for ->
  }

  // Check for array subscript (which is also a form of pointer dereference)
  if (const auto *ASE = dyn_cast<ArraySubscriptExpr>(E)) {
    return ASE->getBase()->getType()->isPointerType();
  }

  return false;
}

/// Check if the type is a polymorphic class type
static bool isPolymorphicType(QualType Type) {
  if (const auto *RT = Type->getAs<RecordType>()) {
    if (const auto *CXXRecord = dyn_cast<CXXRecordDecl>(RT->getDecl())) {
      return CXXRecord->isPolymorphic();
    }
  }
  return false;
}

void AvoidTypeidOnDereferencedPointerCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Typeid = Result.Nodes.getNodeAs<CXXTypeidExpr>("typeid");
  if (!Typeid)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Typeid->getBeginLoc()))
    return;

  // Check if typeid is operating on a type (not an expression)
  if (Typeid->isTypeOperand())
    return;

  // Get the expression operand
  const Expr *Operand = Typeid->getExprOperand();
  if (!Operand)
    return;

  // Check if the operand involves dereferencing a pointer
  if (!isDereferenceOfPointer(Operand))
    return;

  // Get the type of the dereferenced expression
  QualType OperandType = Operand->getType();

  // Check if the type is polymorphic
  if (!isPolymorphicType(OperandType))
    return;

  // Issue a warning
  diag(Typeid->getBeginLoc(),
       "typeid should not be used with a polymorphic type operand that is a "
       "dereferenced pointer; use typeid with a reference or value instead")
      << Operand->getSourceRange();
}

} // namespace clang::tidy::automotive::cpp23

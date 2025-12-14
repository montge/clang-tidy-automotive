//===--- AvoidIncompleteInitializationCheck.cpp - clang-tidy --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidIncompleteInitializationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
/// Returns true if the initializer list is a single {0} (zero-initialization).
/// Uses the syntactic form to check what the user actually wrote.
bool isZeroInitializer(const InitListExpr *Init, ASTContext &Context) {
  // Get the syntactic form (what the user wrote)
  const InitListExpr *SyntacticForm =
      Init->isSyntacticForm() ? Init : Init->getSyntacticForm();
  if (!SyntacticForm)
    SyntacticForm = Init;

  if (SyntacticForm->getNumInits() != 1)
    return false;
  const auto *FirstInit = SyntacticForm->getInit(0);
  if (!FirstInit)
    return false;
  auto Value = FirstInit->getIntegerConstantExpr(Context);
  return Value && *Value == 0;
}

/// Counts explicit initializers (excludes ImplicitValueInitExpr).
unsigned countExplicitInits(const InitListExpr *Init) {
  unsigned Count = 0;
  for (unsigned I = 0; I < Init->getNumInits(); ++I) {
    if (!isa<ImplicitValueInitExpr>(Init->getInit(I)))
      ++Count;
  }
  return Count;
}
} // namespace

void AvoidIncompleteInitializationCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with initializer lists
  Finder->addMatcher(
      varDecl(hasInitializer(initListExpr().bind("init"))).bind("var"), this);
}

void AvoidIncompleteInitializationCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *Init = Result.Nodes.getNodeAs<InitListExpr>("init");
  if (!VD || !Init)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Get the semantic form of the initializer list
  const InitListExpr *SemaInit =
      Init->isSemanticForm() ? Init : Init->getSemanticForm();
  if (!SemaInit)
    SemaInit = Init;

  QualType VarType = VD->getType().getCanonicalType();

  // Count only explicit initializers (semantic form may include implicit ones)
  unsigned NumExplicitInits = countExplicitInits(SemaInit);

  // Check array types
  if (const auto *AT = dyn_cast<ConstantArrayType>(VarType.getTypePtr())) {
    uint64_t ArraySize = AT->getSize().getZExtValue();

    // If some but not all elements are initialized, warn
    // Allow {0} as a special case for zero-initialization
    if (NumExplicitInits > 0 && NumExplicitInits < ArraySize &&
        !isZeroInitializer(SemaInit, *Result.Context)) {
      diag(Init->getBeginLoc(),
           "array has %0 elements but only %1 initializers provided; all "
           "elements should be explicitly initialized")
          << static_cast<unsigned>(ArraySize) << NumExplicitInits;
    }
    return;
  }

  // Check struct/union types
  if (const auto *RT = VarType->getAs<RecordType>()) {
    const RecordDecl *RD = RT->getDecl();
    if (!RD->isCompleteDefinition())
      return;

    unsigned FieldCount = getRecordFieldCount(RT);

    // If some but not all fields are initialized, warn
    // Allow {0} as special case for zero-initialization
    if (NumExplicitInits > 0 && NumExplicitInits < FieldCount &&
        !isZeroInitializer(SemaInit, *Result.Context)) {
      diag(Init->getBeginLoc(),
           "aggregate has %0 members but only %1 initializers provided; all "
           "members should be explicitly initialized")
          << FieldCount << NumExplicitInits;
    }
  }
}

unsigned AvoidIncompleteInitializationCheck::getRecordFieldCount(
    const RecordType *RT) const {
  unsigned Count = 0;
  const RecordDecl *RD = RT->getDecl();
  for (const auto *Field : RD->fields()) {
    (void)Field;
    ++Count;
  }
  return Count;
}

} // namespace clang::tidy::automotive

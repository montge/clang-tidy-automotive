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

  // Check array types
  if (const auto *AT = dyn_cast<ConstantArrayType>(VarType.getTypePtr())) {
    uint64_t ArraySize = AT->getSize().getZExtValue();
    unsigned NumInits = SemaInit->getNumInits();

    // If some but not all elements are initialized, warn
    if (NumInits > 0 && NumInits < ArraySize) {
      // Allow {0} as a special case for zero-initialization
      if (NumInits == 1) {
        if (const auto *FirstInit = SemaInit->getInit(0)) {
          if (auto Value = FirstInit->getIntegerConstantExpr(*Result.Context)) {
            if (*Value == 0) {
              return; // {0} is acceptable
            }
          }
        }
      }
      diag(Init->getBeginLoc(),
           "array has %0 elements but only %1 initializers provided; all "
           "elements should be explicitly initialized")
          << static_cast<unsigned>(ArraySize) << NumInits;
    }
    return;
  }

  // Check struct/union types
  if (const auto *RT = VarType->getAs<RecordType>()) {
    const RecordDecl *RD = RT->getDecl();
    if (!RD->isCompleteDefinition())
      return;

    unsigned FieldCount = getRecordFieldCount(RT);
    unsigned NumInits = SemaInit->getNumInits();

    // If some but not all fields are initialized, warn
    if (NumInits > 0 && NumInits < FieldCount) {
      // Allow {0} as special case
      if (NumInits == 1) {
        if (const auto *FirstInit = SemaInit->getInit(0)) {
          if (auto Value = FirstInit->getIntegerConstantExpr(*Result.Context)) {
            if (*Value == 0) {
              return; // {0} is acceptable
            }
          }
        }
      }
      diag(Init->getBeginLoc(),
           "aggregate has %0 members but only %1 initializers provided; all "
           "members should be explicitly initialized")
          << FieldCount << NumInits;
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

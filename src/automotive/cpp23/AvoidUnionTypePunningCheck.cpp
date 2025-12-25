//===--- AvoidUnionTypePunningCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnionTypePunningCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {

// Check if two types are compatible for union type-punning
// (e.g., different integer types of same size might be compatible)
bool areTypesCompatibleForUnion(QualType T1, QualType T2) {
  // Same type is always compatible (not type-punning)
  if (T1.getCanonicalType() == T2.getCanonicalType())
    return true;

  // Character types can alias anything (common-initial-sequence exception)
  if (T1->isCharType() || T2->isCharType())
    return true;

  // Signed/unsigned variants of same type
  if (T1->isSignedIntegerType() && T2->isUnsignedIntegerType()) {
    return T1.getCanonicalType()
               .getUnqualifiedType()
               .withoutLocalFastQualifiers() ==
           T2.getCanonicalType()
               .getUnqualifiedType()
               .withoutLocalFastQualifiers();
  }
  if (T1->isUnsignedIntegerType() && T2->isSignedIntegerType()) {
    return T1.getCanonicalType()
               .getUnqualifiedType()
               .withoutLocalFastQualifiers() ==
           T2.getCanonicalType()
               .getUnqualifiedType()
               .withoutLocalFastQualifiers();
  }

  return false;
}

// Get a unique identifier for a union variable
std::string getUnionVarKey(const Expr *Base, ASTContext *Context) {
  if (const auto *DRE = dyn_cast<DeclRefExpr>(Base->IgnoreParenImpCasts())) {
    if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
      // Use address of the VarDecl as a unique key
      return VD->getNameAsString() + "@" +
             std::to_string(reinterpret_cast<uintptr_t>(VD));
    }
  }
  return "";
}

} // namespace

void AvoidUnionTypePunningCheck::registerMatchers(MatchFinder *Finder) {
  // Match member expressions that access union fields in write context
  Finder->addMatcher(
      binaryOperator(
          isAssignmentOperator(),
          hasLHS(memberExpr(member(fieldDecl().bind("write_field")),
                            hasObjectExpression(
                                hasType(recordDecl(isUnion()).bind("union"))))
                     .bind("write_member")))
          .bind("write_op"),
      this);

  // Match member expressions that access union fields in read context
  Finder->addMatcher(memberExpr(unless(hasParent(binaryOperator(
                                    isAssignmentOperator(),
                                    hasLHS(memberExpr().bind("ignore_lhs"))))),
                                member(fieldDecl().bind("read_field")),
                                hasObjectExpression(hasType(
                                    recordDecl(isUnion()).bind("union"))))
                         .bind("read_member"),
                     this);
}

void AvoidUnionTypePunningCheck::check(const MatchFinder::MatchResult &Result) {

  // Handle write operations
  if (const auto *WriteOp =
          Result.Nodes.getNodeAs<BinaryOperator>("write_op")) {
    if (Result.SourceManager->isInSystemHeader(WriteOp->getOperatorLoc()))
      return;

    const auto *WriteMember =
        Result.Nodes.getNodeAs<MemberExpr>("write_member");
    const auto *WriteField = Result.Nodes.getNodeAs<FieldDecl>("write_field");

    if (!WriteMember || !WriteField)
      return;

    // Get the base union variable
    std::string UnionKey =
        getUnionVarKey(WriteMember->getBase(), Result.Context);
    if (UnionKey.empty())
      return;

    // Record which field was written to
    LastWrittenMember[UnionKey] = WriteField->getNameAsString();
    return;
  }

  // Handle read operations
  if (const auto *ReadMember =
          Result.Nodes.getNodeAs<MemberExpr>("read_member")) {
    if (Result.SourceManager->isInSystemHeader(ReadMember->getExprLoc()))
      return;

    // Ignore if this is the LHS of an assignment (covered by write case)
    if (Result.Nodes.getNodeAs<MemberExpr>("ignore_lhs"))
      return;

    const auto *ReadField = Result.Nodes.getNodeAs<FieldDecl>("read_field");
    if (!ReadField)
      return;

    // Get the base union variable
    std::string UnionKey =
        getUnionVarKey(ReadMember->getBase(), Result.Context);
    if (UnionKey.empty())
      return;

    // Check if this union variable was previously written to
    auto It = LastWrittenMember.find(UnionKey);
    if (It == LastWrittenMember.end())
      return; // No previous write recorded, could be uninitialized

    const std::string &WrittenFieldName = It->second;
    const std::string ReadFieldName = ReadField->getNameAsString();

    // If reading from a different member than was written
    if (WrittenFieldName != ReadFieldName) {
      // Find the written field to compare types
      const auto *Union = Result.Nodes.getNodeAs<RecordDecl>("union");
      if (!Union)
        return;

      FieldDecl *WrittenField = nullptr;
      for (auto *Field : Union->fields()) {
        if (Field->getNameAsString() == WrittenFieldName) {
          WrittenField = Field;
          break;
        }
      }

      if (!WrittenField)
        return;

      // Check if the types are incompatible
      QualType WrittenType = WrittenField->getType();
      QualType ReadType = ReadField->getType();

      if (!areTypesCompatibleForUnion(WrittenType, ReadType)) {
        diag(ReadMember->getExprLoc(),
             "union member '%0' accessed after writing to member '%1' "
             "(type-punning from %2 to %3)")
            << ReadFieldName << WrittenFieldName << WrittenType.getAsString()
            << ReadType.getAsString();
        diag(ReadMember->getExprLoc(),
             "use std::bit_cast or memcpy for type conversion",
             DiagnosticIDs::Note);
      }
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

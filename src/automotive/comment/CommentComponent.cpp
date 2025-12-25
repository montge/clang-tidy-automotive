//===--- CommentComponent.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CommentComponent.h"
#include "AvoidCommentWithinCommentCheck.h"
#include "AvoidLinesplicingWithinCommentCheck.h"

namespace clang::tidy::automotive {

void CommentComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidCommentWithinCommentCheck>(
      "automotive-avoid-comment-within-comment");
  CheckFactories.registerCheck<AvoidLinesplicingWithinCommentCheck>(
      "automotive-avoid-line-splicing-within-comment");

  // MISRA C:2025 Rule 3.1 - Comment within comment (Required)
  CheckFactories.registerCheck<AvoidCommentWithinCommentCheck>(
      "automotive-c25-req-3.1");

  // MISRA C:2025 Rule 3.2 - Line splicing within comment (Required)
  CheckFactories.registerCheck<AvoidLinesplicingWithinCommentCheck>(
      "automotive-c25-req-3.2");
}

} // namespace clang::tidy::automotive

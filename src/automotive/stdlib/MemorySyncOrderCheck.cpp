//===--- MemorySyncOrderCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MemorySyncOrderCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

bool MemorySyncOrderCheck::isNonSeqCstMemoryOrder(const Expr *E) const {
  if (!E)
    return false;

  // Strip implicit casts and parentheses
  E = E->IgnoreParenImpCasts();

  // Check for DeclRefExpr referring to memory_order constants
  if (const auto *DRE = dyn_cast<DeclRefExpr>(E)) {
    StringRef Name = DRE->getDecl()->getName();

    // memory_order_seq_cst is the only acceptable ordering
    if (Name == "memory_order_seq_cst")
      return false;

    // All other memory_order_* values are non-seq_cst
    if (Name.starts_with("memory_order_"))
      return true;
  }

  // Check for integer literals that correspond to memory_order values
  // memory_order enum values: relaxed=0, consume=1, acquire=2, release=3,
  // acq_rel=4, seq_cst=5
  if (const auto *IL = dyn_cast<IntegerLiteral>(E)) {
    llvm::APInt Value = IL->getValue();
    // seq_cst is 5 (__ATOMIC_SEQ_CST), all others are violations
    if (Value != 5)
      return true;
  }

  return false;
}

void MemorySyncOrderCheck::registerMatchers(MatchFinder *Finder) {
  // Match atomic expressions - C11 atomic operations use AtomicExpr
  // Don't use unless(isExpansionInSystemHeader()) because we want to catch
  // user code that calls atomic macros which expand in system headers
  Finder->addMatcher(atomicExpr().bind("atomicOp"), this);

  // Fence functions are not AtomicExpr, they're regular function calls
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName(
                   "atomic_thread_fence", "atomic_signal_fence",
                   "__c11_atomic_thread_fence", "__c11_atomic_signal_fence"))))
          .bind("fenceCall"),
      this);
}

void MemorySyncOrderCheck::check(const MatchFinder::MatchResult &Result) {
  // Check for fence function calls
  if (const auto *FenceCall = Result.Nodes.getNodeAs<CallExpr>("fenceCall")) {
    // Get the spelling location of the first argument
    if (FenceCall->getNumArgs() > 0) {
      SourceLocation CallLoc = Result.SourceManager->getSpellingLoc(
          FenceCall->getArg(0)->getBeginLoc());

      if (!Result.SourceManager->isInSystemHeader(CallLoc)) {
        const Expr *MemOrder = FenceCall->getArg(0);
        if (MemOrder && isNonSeqCstMemoryOrder(MemOrder)) {
          diag(CallLoc,
               "atomic fence operation shall use memory_order_seq_cst");
        }
      }
    }
    return;
  }

  const auto *AE = Result.Nodes.getNodeAs<AtomicExpr>("atomicOp");
  if (!AE)
    return;

  // For atomic operations that come from macros, we need to check where the
  // arguments were written (not where the macro expands)
  // Get the pointer argument's spelling location as a proxy for where the call
  // was made
  const Expr *Ptr = AE->getPtr();
  if (!Ptr)
    return;

  SourceLocation CallLoc =
      Result.SourceManager->getSpellingLoc(Ptr->getBeginLoc());

  // Skip if the call is in system header
  if (Result.SourceManager->isInSystemHeader(CallLoc))
    return;

  // Check if this is an operation that uses explicit memory ordering
  // Operations like atomic_init don't have memory_order parameters
  StringRef OpName = AE->getOpAsString();

  // Skip operations without memory order parameter
  if (OpName == "__c11_atomic_init" || OpName == "__opencl_atomic_init")
    return;

  // Get the memory order argument
  const Expr *Order = AE->getOrder();
  if (Order && isNonSeqCstMemoryOrder(Order)) {
    diag(CallLoc, "atomic operation shall use memory_order_seq_cst");
  }

  // For compare_exchange operations, also check the failure ordering
  if (AE->isCmpXChg()) {
    const Expr *OrderFail = AE->getOrderFail();
    if (OrderFail && isNonSeqCstMemoryOrder(OrderFail)) {
      diag(CallLoc, "atomic operation shall use memory_order_seq_cst for "
                    "failure ordering");
    }
  }
}

} // namespace clang::tidy::automotive

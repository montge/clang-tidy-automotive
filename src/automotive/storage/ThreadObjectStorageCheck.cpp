//===--- ThreadObjectStorageCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ThreadObjectStorageCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
// Thread object type names from C11 threads.h and POSIX
const llvm::StringSet<> ThreadTypeNames = {
    "thrd_t",          "mtx_t",          "cnd_t",           "tss_t",
    "__pthread_mutex_s", "pthread_mutex_t", "pthread_cond_t", "pthread_t",
    "pthread_rwlock_t"};

bool isThreadType(QualType QT) {
  // Get the canonical type name
  std::string TypeName = QT.getUnqualifiedType().getAsString();

  // Check if it's directly one of our thread types
  if (ThreadTypeNames.contains(TypeName))
    return true;

  // Also check the desugared type for typedef chains
  QualType DesugaredType = QT.getCanonicalType();
  if (const auto *RT = DesugaredType->getAs<RecordType>()) {
    if (const auto *RD = RT->getDecl()) {
      StringRef RecordName = RD->getName();
      if (ThreadTypeNames.contains(RecordName))
        return true;
    }
  }

  return false;
}
} // namespace

void ThreadObjectStorageCheck::registerMatchers(MatchFinder *Finder) {
  // Match all variable declarations; we'll filter by type in check()
  // This is necessary because thread types can be typedefs to anonymous structs
  Finder->addMatcher(varDecl().bind("var"), this);
}

void ThreadObjectStorageCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!Var)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Var->getLocation()))
    return;

  // Check if this is a thread synchronization type
  if (!isThreadType(Var->getType()))
    return;

  // Check storage duration
  StorageDuration SD = Var->getStorageDuration();

  // Compliant: static storage duration (global, static local, or extern)
  if (SD == SD_Static)
    return;

  // Non-compliant: automatic storage duration (local variable without static)
  if (SD == SD_Automatic) {
    diag(Var->getLocation(),
         "thread synchronization object '%0' has automatic storage duration; "
         "use static storage to ensure lifetime across thread operations")
        << Var->getName();
    return;
  }

  // Non-compliant: thread storage duration (_Thread_local)
  if (SD == SD_Thread) {
    diag(Var->getLocation(),
         "thread synchronization object '%0' has thread storage duration; "
         "use static storage to ensure shared access across threads")
        << Var->getName();
    return;
  }
}

} // namespace clang::tidy::automotive

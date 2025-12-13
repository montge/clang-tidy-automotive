//===--- AvoidStdlibMemoryCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBMEMORYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBMEMORYCHECK_H

#include "AvoidApiCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-stdlib
/// @brief Detects usage of dynamic memory allocation functions from stdlib.h.
///
/// Dynamic memory allocation (malloc, calloc, realloc, free) can lead to
/// memory leaks, fragmentation, and allocation failures that are difficult
/// to handle. In safety-critical systems, static memory allocation is
/// preferred for predictable behavior and deterministic execution.
///
/// @par MISRA C:2025 Rule 21.3
/// The memory allocation and deallocation functions of <stdlib.h> shall not
/// be used.
/// @par Category: Required
///
/// Example:
/// \code
///   int *p = malloc(sizeof(int));  // Warning: malloc usage
///   free(p);                       // Warning: free usage
/// \endcode
class AvoidStdlibMemoryCheck : public AvoidApiCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidStdlibMemoryCheck(StringRef Name, ClangTidyContext *Context)
      : AvoidApiCheck(
            Name, Context, "pelle.h",
            {"calloc", "malloc", "realloc", "aligned_alloc", "free"}) {}
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBMEMORYCHECK_H

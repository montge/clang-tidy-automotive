//===--- AvoidStdlibMemoryCheck.h - clang-tidy -----------------*- C++ -*-===//
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

/// Detects usage of dynamic memory allocation functions.
///
/// Dynamic memory allocation functions (malloc, calloc, realloc, free,
/// aligned_alloc) can lead to memory leaks, fragmentation, and timing
/// unpredictability. They are prohibited in safety-critical code.
///
/// Related MISRA C:2025 Rule: 21.3 - The memory allocation and deallocation
/// functions of <stdlib.h> shall not be used.
///
/// Related MISRA C:2025 Directive: 4.12 - Dynamic memory allocation shall
/// not be used.
///
/// Example:
/// \code
///   void *p = malloc(100);  // Warning: use of malloc
///   free(p);                // Warning: use of free
/// \endcode
class AvoidStdlibMemoryCheck : public AvoidApiCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidStdlibMemoryCheck(StringRef Name, ClangTidyContext *Context);

  /// Destructor.
  ~AvoidStdlibMemoryCheck() override = default;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBMEMORYCHECK_H

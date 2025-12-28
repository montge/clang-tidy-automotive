//===--- StdlibComponent.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StdlibComponent.h"
#include "AtoXCheck.h"
#include "AvoidBsearchQsortCheck.h"
#include "AvoidCtypeHeaderCheck.h"
#include "AvoidMemcmpOnStringsCheck.h"
#include "AvoidNestedSyncCallsCheck.h"
#include "AvoidSetjmpHeaderCheck.h"
#include "AvoidSignalHeaderCheck.h"
#include "AvoidStdargHeaderCheck.h"
#include "AvoidStdioHeaderCheck.h"
#include "AvoidStdlibMemoryCheck.h"
#include "AvoidStdlibRandCheck.h"
#include "AvoidTgmathHeaderCheck.h"
#include "AvoidTimeHeaderCheck.h"
#include "AvoidWcharHeaderCheck.h"
#include "AvoidstdlibsystemcallCheck.h"
#include "CondVarMutexAssociationCheck.h"
#include "ErrnoTestingCheck.h"
#include "ErrnoUsageCheck.h"
#include "ExitCheck.h"
#include "MemorySyncOrderCheck.h"
#include "RecursiveMutexLockCheck.h"
#include "TgmathConsistentTypeCheck.h"
#include "TgmathOperandTypeCheck.h"
#include "ThreadJoinDetachCheck.h"
#include "ThreadSpecificStorageCheck.h"
#include "TssUsageCheck.h"
#include "UnsynchronizedAccessCheck.h"
#include "MutexUnlockThreadCheck.h"
#include "MtxTimedlockTypeCheck.h"
#include "SpuriousWakeupCheck.h"
#include "WrongSizetArgumentCheck.h"

namespace clang::tidy::automotive {

void StdlibComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidStdargHeaderCheck>(
      "automotive-avoid-stdarg-header");
  CheckFactories.registerCheck<AvoidSetjmpHeaderCheck>(
      "automotive-avoid-setjmp-header");
  CheckFactories.registerCheck<AvoidSignalHeaderCheck>(
      "automotive-avoid-signal-header");
  CheckFactories.registerCheck<AtoXCheck>("automotive-avoid-ascii-to-number");
  CheckFactories.registerCheck<ExitCheck>("automotive-avoid-stdlib-exit");

  CheckFactories.registerCheck<AvoidstdlibsystemcallCheck>(
      "automotive-avoid-stdlib-system");
  CheckFactories.registerCheck<AvoidStdlibRandCheck>(
      "automotive-avoid-stdlib-rand");

  // Rule 21.24 - Random number generator functions (Required)
  // Extends 21.12 (rand) to include srand
  CheckFactories.registerCheck<AvoidStdlibRandCheck>(
      "automotive-c23-req-21.24");

  // Rule 21.3 / Dir 4.12 - Dynamic memory allocation (Required)
  CheckFactories.registerCheck<AvoidStdlibMemoryCheck>(
      "automotive-c23-req-21.3");

  // Rule 21.6 - Standard library I/O functions (Required)
  CheckFactories.registerCheck<AvoidStdioHeaderCheck>(
      "automotive-c23-req-21.6");

  // Rule 21.10 - Standard library time functions (Required)
  CheckFactories.registerCheck<AvoidTimeHeaderCheck>(
      "automotive-c23-req-21.10");

  // Rule 21.21 - Wide character functions (Required)
  CheckFactories.registerCheck<AvoidWcharHeaderCheck>(
      "automotive-c23-req-21.21");

  // Rule 21.11 - Type-generic math header (Required)
  CheckFactories.registerCheck<AvoidTgmathHeaderCheck>(
      "automotive-c23-req-21.11");

  // Rule 21.13 - ctype.h functions (Mandatory)
  CheckFactories.registerCheck<AvoidCtypeHeaderCheck>(
      "automotive-c23-mand-21.13");

  // Rule 21.9 - bsearch/qsort prohibition (Required)
  CheckFactories.registerCheck<AvoidBsearchQsortCheck>(
      "automotive-c23-req-21.9");

  // Rule 22.8 - errno shall be zero before call (Required)
  // Rule 22.9 - errno shall be tested after call (Required)
  // Combined check for errno-setting function usage
  CheckFactories.registerCheck<ErrnoUsageCheck>("automotive-c25-req-22.8");
  CheckFactories.registerCheck<ErrnoUsageCheck>("automotive-c25-req-22.9");

  // Rule 22.10 - errno testing (Required)
  // errno shall only be tested after an errno-setting function
  CheckFactories.registerCheck<ErrnoTestingCheck>("automotive-c23-req-22.10");

  // Rule 21.22 - tgmath.h operand types (Mandatory)
  // All operand arguments to type-generic macros shall have appropriate type
  CheckFactories.registerCheck<TgmathOperandTypeCheck>(
      "automotive-c25-mand-21.22");

  // Rule 21.23 - tgmath consistent types (Required)
  // All operand arguments to multi-argument tgmath macros shall have same type
  CheckFactories.registerCheck<TgmathConsistentTypeCheck>(
      "automotive-c25-req-21.23");

  // Rule 21.25 - Memory synchronization order (Required)
  // All memory synchronization operations shall be executed in sequentially
  // consistent order
  CheckFactories.registerCheck<MemorySyncOrderCheck>(
      "automotive-c25-req-21.25");

  // Rule 21.14 - memcmp shall not be used for string comparison (Required)
  CheckFactories.registerCheck<AvoidMemcmpOnStringsCheck>(
      "automotive-c25-req-21.14");

  // Rule 22.11 - A thread that was previously joined or detached shall not
  // be subsequently joined or detached (Required)
  CheckFactories.registerCheck<ThreadJoinDetachCheck>(
      "automotive-c25-req-22.11");

  // Rule 22.13 - A mutex shall not be locked more than once by a thread
  // (Required) - detects recursive locking of non-recursive mutexes
  CheckFactories.registerCheck<RecursiveMutexLockCheck>(
      "automotive-c25-req-22.13");

  // Rule 22.15 - Thread-specific storage pointers shall be retrieved from
  // the same thread that created them (Required)
  CheckFactories.registerCheck<ThreadSpecificStorageCheck>(
      "automotive-c25-req-22.15");

  // Rule 22.16 - An object shall not be accessed from competing threads
  // without synchronization (Required)
  CheckFactories.registerCheck<UnsynchronizedAccessCheck>(
      "automotive-c25-req-22.16");

  // Rule 22.17 - A mutex shall be unlocked from the same thread that locked it
  // (Required)
  CheckFactories.registerCheck<MutexUnlockThreadCheck>(
      "automotive-c25-req-22.17");

  // Rule 22.18 - A non-recursive mutex shall not be recursively locked
  // (Mandatory)
  CheckFactories.registerCheck<RecursiveMutexLockCheck>(
      "automotive-c25-mand-22.18");

  // Rule 22.19 - A condition variable shall be associated with at most one
  // mutex object (Required)
  CheckFactories.registerCheck<CondVarMutexAssociationCheck>(
      "automotive-c25-req-22.19");

  // Rule 22.20 - Spurious wakeups shall be handled correctly (Required)
  CheckFactories.registerCheck<SpuriousWakeupCheck>(
      "automotive-c25-req-22.20");

  // MISRA C:2025 Rule 21.1 - #define/#undef reserved identifiers (Required)
  // Note: Registered in PreprocessorComponent

  // MISRA C:2025 Rule 21.3 - Dynamic memory allocation (Required)
  CheckFactories.registerCheck<AvoidStdlibMemoryCheck>(
      "automotive-c25-req-21.3");

  // MISRA C:2025 Rule 21.4 - setjmp header (Required)
  CheckFactories.registerCheck<AvoidSetjmpHeaderCheck>(
      "automotive-c25-req-21.4");

  // MISRA C:2025 Rule 21.5 - signal header (Required)
  CheckFactories.registerCheck<AvoidSignalHeaderCheck>(
      "automotive-c25-req-21.5");

  // MISRA C:2025 Rule 21.6 - stdio header I/O functions (Required)
  CheckFactories.registerCheck<AvoidStdioHeaderCheck>(
      "automotive-c25-req-21.6");

  // MISRA C:2025 Rule 21.7 - atof/atoi/atol/atoll (Required)
  CheckFactories.registerCheck<AtoXCheck>("automotive-c25-req-21.7");

  // MISRA C:2025 Rule 21.8 - abort/exit/quick_exit/getenv (Required)
  CheckFactories.registerCheck<ExitCheck>("automotive-c25-req-21.8");

  // MISRA C:2025 Rule 21.9 - bsearch/qsort prohibition (Required)
  CheckFactories.registerCheck<AvoidBsearchQsortCheck>(
      "automotive-c25-req-21.9");

  // MISRA C:2025 Rule 21.10 - time.h functions (Required)
  CheckFactories.registerCheck<AvoidTimeHeaderCheck>(
      "automotive-c25-req-21.10");

  // MISRA C:2025 Rule 21.11 - tgmath.h (Required)
  CheckFactories.registerCheck<AvoidTgmathHeaderCheck>(
      "automotive-c25-req-21.11");

  // MISRA C:2025 Rule 21.12 - rand/srand functions (Required)
  CheckFactories.registerCheck<AvoidStdlibRandCheck>(
      "automotive-c25-req-21.12");

  // MISRA C:2025 Rule 21.13 - ctype.h functions (Mandatory)
  CheckFactories.registerCheck<AvoidCtypeHeaderCheck>(
      "automotive-c25-mand-21.13");

  // MISRA C:2025 Rule 21.17 - system function (Required)
  CheckFactories.registerCheck<AvoidstdlibsystemcallCheck>(
      "automotive-c25-req-21.17");

  // MISRA C:2025 Rule 21.21 - wchar.h functions (Required)
  CheckFactories.registerCheck<AvoidWcharHeaderCheck>(
      "automotive-c25-req-21.21");

  // MISRA C:2025 Rule 17.1 - stdarg.h (Required)
  CheckFactories.registerCheck<AvoidStdargHeaderCheck>(
      "automotive-c25-req-17.1");

  // MISRA C:2025 Rule 22.10 - errno testing (Required)
  CheckFactories.registerCheck<ErrnoTestingCheck>("automotive-c25-req-22.10");

  // MISRA C:2025 Rule 22.12 - Thread-specific storage functions (Required)
  CheckFactories.registerCheck<TssUsageCheck>("automotive-c25-req-22.12");

  // MISRA C:2025 Rule 21.26 - mtx_timedlock mutex type (Required)
  CheckFactories.registerCheck<MtxTimedlockTypeCheck>(
      "automotive-c25-req-21.26");

  // MISRA C:2025 Rule 21.18 - size_t argument validity (Mandatory)
  CheckFactories.registerCheck<WrongSizetArgumentCheck>(
      "automotive-c25-mand-21.18");

  // MISRA C:2025 Rule 22.20 - Thread synchronization nesting (Mandatory)
  CheckFactories.registerCheck<AvoidNestedSyncCallsCheck>(
      "automotive-c25-mand-22.20");
}

} // namespace clang::tidy::automotive

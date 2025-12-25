//===--- AvoidMemberPointerConversionCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDMEMBERPOINTERCONVERSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDMEMBERPOINTERCONVERSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-conversions
/// @brief Detects invalid conversions between pointers to members of different
/// class types.
///
/// Pointer-to-member conversions between unrelated classes can lead to
/// undefined behavior. Only conversions from a pointer-to-member of a derived
/// class to a pointer-to-member of its base class are safe.
///
/// @par MISRA C++:2023 Rule 11.6
/// Invalid conversions between pointers to different member types should be
/// avoided.
/// @par Category: Required
///
/// Example:
/// @code
///   class A { int x; };
///   class B { int y; };
///   int A::* pa = &A::x;
///   int B::* pb = reinterpret_cast<int B::*>(pa);  // Warning: unrelated
///
///   class Base { int x; };
///   class Derived : public Base { int y; };
///   int Derived::* pd = &Derived::y;
///   int Base::* pb = static_cast<int Base::*>(pd);  // OK: derived to base
/// @endcode
class AvoidMemberPointerConversionCheck : public ClangTidyCheck {
public:
  AvoidMemberPointerConversionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidMemberPointerConversionCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Checks if a conversion between member pointer types is valid.
  /// Returns true if the conversion is from derived to base (safe).
  bool isValidMemberPointerConversion(const CXXRecordDecl *FromClass,
                                      const CXXRecordDecl *ToClass) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDMEMBERPOINTERCONVERSIONCHECK_H

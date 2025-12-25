//===--- AvoidTypeidOnDereferencedPointerCheck.h - clang-tidy --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTYPEIDONDEREFERENCEDPOINTERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTYPEIDONDEREFERENCEDPOINTERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-expressions
/// @brief Detects usage of typeid with a polymorphic type operand that is a
/// dereferenced pointer.
///
/// When typeid is applied to a dereferenced pointer to a polymorphic type,
/// the operand is evaluated at runtime to determine the dynamic type. If the
/// pointer is null, this will throw std::bad_typeid. This runtime evaluation
/// introduces unpredictability in safety-critical systems.
///
/// @par MISRA C++:2023 Rule 17.3.1
/// The typeid operator should not be used with a polymorphic type operand
/// that is a dereferenced pointer.
/// @par Category: Advisory
///
/// Example:
/// @code
///   Base* ptr = getDerivedObject();
///   typeid(*ptr);  // Warning: typeid on dereferenced pointer to polymorphic
///                  // type
///   typeid(ptr->member);  // Warning: accessing through dereferenced pointer
/// @endcode
///
/// Compliant examples:
/// @code
///   Base& ref = *ptr;
///   typeid(ref);  // OK: using reference
///
///   Base obj;
///   typeid(obj);  // OK: using value
///
///   typeid(Base);  // OK: using type name
///
///   NonPolymorphic* ptr2 = getObject();
///   typeid(*ptr2);  // OK: non-polymorphic type
/// @endcode
class AvoidTypeidOnDereferencedPointerCheck : public ClangTidyCheck {
public:
  AvoidTypeidOnDereferencedPointerCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidTypeidOnDereferencedPointerCheck() override;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTYPEIDONDEREFERENCEDPOINTERCHECK_H

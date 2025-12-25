//===--- AvoidTypePunningCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTYPEPUNNINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTYPEPUNNINGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects type-punning through pointer or reference casts.
///
/// MISRA C++:2023 Rule 5.13: An object shall not be accessed through a pointer
/// or reference to an incompatible type (type-punning).
///
/// Type-punning is the practice of accessing an object of one type through a
/// pointer or reference to a different, incompatible type. This violates the
/// strict aliasing rule and leads to undefined behavior. It typically occurs
/// through reinterpret_cast or C-style casts followed by dereference.
///
/// Example violations:
/// @code
///   int x = 42;
///   float* fp = reinterpret_cast<float*>(&x);
///   float f = *fp;  // Warning: type-punning through incompatible pointer
///
///   struct A { int i; };
///   struct B { int i; };
///   A a{10};
///   B* bp = reinterpret_cast<B*>(&a);
///   int val = bp->i;  // Warning: accessing A through B pointer
///
///   char buffer[4];
///   int* ip = (int*)buffer;
///   *ip = 100;  // Warning: type-punning
/// @endcode
///
/// Compliant:
/// @code
///   int x = 42;
///   float f;
///   std::memcpy(&f, &x, sizeof(f));  // OK - using memcpy
///
///   int x = 42;
///   char* cp = reinterpret_cast<char*>(&x);
///   char c = *cp;  // OK - char* exception
/// @endcode
class AvoidTypePunningCheck : public ClangTidyCheck {
public:
  AvoidTypePunningCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

private:
  /// Checks if two types are related (compatible for type-punning exceptions).
  /// Returns true if the types are compatible (e.g., char*, signed char*,
  /// unsigned char*, void*, or derived/base class relationships).
  bool areTypesCompatible(QualType FromType, QualType ToType,
                          const ASTContext &Ctx) const;

  /// Checks if a type is a character type (char, signed char, unsigned char).
  bool isCharacterType(QualType Type) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTYPEPUNNINGCHECK_H

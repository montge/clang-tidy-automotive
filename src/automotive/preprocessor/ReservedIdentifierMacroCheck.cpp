//===--- ReservedIdentifierMacroCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ReservedIdentifierMacroCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringSet.h"
#include <cctype>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Standard library reserved names that should not be defined/undefined
const llvm::StringSet<> ReservedNames = {
    // Standard macros
    "assert", "errno", "math_errhandling", "setjmp", "va_arg", "va_copy",
    "va_end", "va_start", "offsetof", "NULL", "true", "false", "bool",

    // Standard types
    "FILE", "fpos_t", "size_t", "ptrdiff_t", "wchar_t", "max_align_t", "wint_t",
    "mbstate_t",

    // Fixed-width integer types
    "int8_t", "int16_t", "int32_t", "int64_t", "uint8_t", "uint16_t",
    "uint32_t", "uint64_t", "intptr_t", "uintptr_t", "intmax_t", "uintmax_t",

    // Limit macros
    "CHAR_BIT", "CHAR_MAX", "CHAR_MIN", "INT_MAX", "INT_MIN", "LONG_MAX",
    "LONG_MIN", "LLONG_MAX", "LLONG_MIN", "SCHAR_MAX", "SCHAR_MIN", "SHRT_MAX",
    "SHRT_MIN", "UCHAR_MAX", "UINT_MAX", "ULONG_MAX", "ULLONG_MAX", "USHRT_MAX",

    // stdio.h macros and file pointers
    "EOF", "SEEK_CUR", "SEEK_END", "SEEK_SET", "BUFSIZ", "FILENAME_MAX",
    "FOPEN_MAX", "TMP_MAX", "stdin", "stdout", "stderr",

    // stdlib.h macros
    "EXIT_FAILURE", "EXIT_SUCCESS", "RAND_MAX", "MB_CUR_MAX", "MB_LEN_MAX",

    // Other standard library names
    "NDEBUG", "EDOM", "ERANGE", "EILSEQ", "FE_ALL_EXCEPT"};

/// Checks if a macro name is reserved by the C standard.
///
/// A name is reserved if:
/// - It starts with underscore followed by uppercase letter (_[A-Z])
/// - It starts with double underscore (__)
/// - It is a standard library reserved name
bool isReservedIdentifier(StringRef Name) {
  if (Name.empty())
    return false;

  // Check for underscore followed by uppercase or another underscore
  if (Name.size() >= 2 && Name[0] == '_') {
    if (std::isupper(static_cast<unsigned char>(Name[1])) || Name[1] == '_')
      return true;
  }

  // Check for known standard library names
  return ReservedNames.contains(Name);
}

class ReservedIdentifierMacroPPCallbacks : public PPCallbacks {
public:
  ReservedIdentifierMacroPPCallbacks(ClangTidyCheck &Check,
                                     const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    SourceLocation Loc = MacroNameTok.getLocation();

    // Skip macros with invalid locations (compiler built-ins)
    if (Loc.isInvalid())
      return;

    // Skip macros defined in system headers
    if (SM.isInSystemHeader(Loc))
      return;

    // Skip compiler built-in macros from predefines or command line buffers
    if (SM.isWrittenInBuiltinFile(Loc) || SM.isWrittenInCommandLineFile(Loc))
      return;

    // Skip macros from non-file sources (empty filename = compiler-internal)
    StringRef FileName = SM.getFilename(Loc);
    if (FileName.empty())
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
    if (isReservedIdentifier(MacroName)) {
      Check.diag(MacroNameTok.getLocation(),
                 "#define of reserved identifier '%0' violates MISRA C:2025 "
                 "Rule 20.15")
          << MacroName;
    }
  }

  void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                      const MacroDirective *Undef) override {
    SourceLocation Loc = MacroNameTok.getLocation();

    // Skip undefs with invalid locations
    if (Loc.isInvalid())
      return;

    // Skip undefs in system headers
    if (SM.isInSystemHeader(Loc))
      return;

    // Skip undefs from predefines or command line buffers
    if (SM.isWrittenInBuiltinFile(Loc) || SM.isWrittenInCommandLineFile(Loc))
      return;

    // Skip undefs from non-file sources
    StringRef FileName = SM.getFilename(Loc);
    if (FileName.empty())
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
    if (isReservedIdentifier(MacroName)) {
      Check.diag(MacroNameTok.getLocation(),
                 "#undef of reserved identifier '%0' violates MISRA C:2025 "
                 "Rule 20.15")
          << MacroName;
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
};

} // anonymous namespace

void ReservedIdentifierMacroCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<ReservedIdentifierMacroPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive

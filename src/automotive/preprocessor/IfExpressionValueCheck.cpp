//===--- IfExpressionValueCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IfExpressionValueCheck.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Determines if an expression naturally evaluates to a boolean (0 or 1).
/// This checks the syntactic structure to see if the expression is guaranteed
/// to produce only 0 or 1 as its result.
bool isBooleanExpression(const SourceManager &SM, Preprocessor *PP,
                         SourceRange ConditionRange) {
  if (ConditionRange.isInvalid())
    return false;

  // Get the source text of the condition using Lexer::getSourceText
  // which properly handles token ranges
  CharSourceRange CharRange = CharSourceRange::getTokenRange(ConditionRange);
  StringRef ConditionText = Lexer::getSourceText(CharRange, SM, LangOptions());

  // Trim whitespace
  ConditionText = ConditionText.trim();

  // Check for literal 0 or 1
  if (ConditionText == "0" || ConditionText == "1")
    return true;

  // Check for numeric literals that are not 0 or 1
  // This includes decimal (2, 3, -1), hex (0xFF), octal (077), etc.
  if (!ConditionText.empty()) {
    bool IsNumericLiteral = false;
    size_t StartIdx = 0;

    // Handle negative numbers
    if (ConditionText[0] == '-' || ConditionText[0] == '+') {
      StartIdx = 1;
    }

    // Check if it starts with a digit or 0x/0X (hex) or 0 (octal)
    if (StartIdx < ConditionText.size()) {
      if (std::isdigit(ConditionText[StartIdx])) {
        IsNumericLiteral = true;
        // Verify the rest are valid numeric characters
        for (size_t i = StartIdx; i < ConditionText.size(); ++i) {
          char c = ConditionText[i];
          // Allow digits, hex chars (a-f, A-F), x/X for hex prefix, u/U/l/L for
          // suffixes
          if (!std::isdigit(c) && c != 'x' && c != 'X' && c != 'a' &&
              c != 'b' && c != 'c' && c != 'd' && c != 'e' && c != 'f' &&
              c != 'A' && c != 'B' && c != 'C' && c != 'D' && c != 'E' &&
              c != 'F' && c != 'u' && c != 'U' && c != 'l' && c != 'L') {
            IsNumericLiteral = false;
            break;
          }
        }
      }
    }

    // If it's a numeric literal and not "0" or "1", it's a violation
    if (IsNumericLiteral) {
      return false; // Not a boolean expression
    }
  }

  // Check for common boolean operators that always yield 0 or 1:
  // - Logical operators: !, &&, ||
  // - Comparison operators: ==, !=, <, <=, >, >=
  // - defined() operator

  // Check for defined() - this always yields 0 or 1
  if (ConditionText.starts_with("defined(") ||
      ConditionText.starts_with("defined "))
    return true;

  // Check for logical NOT (!)
  if (ConditionText.starts_with("!"))
    return true;

  // Check for comparison and logical operators
  // Note: We check for operators that guarantee boolean results
  bool HasBooleanOp = false;
  if (ConditionText.contains("==") || ConditionText.contains("!=") ||
      ConditionText.contains("&&") || ConditionText.contains("||") ||
      ConditionText.contains("<=") || ConditionText.contains(">=")) {
    HasBooleanOp = true;
  }

  // Check for shift operators - these do NOT guarantee 0 or 1
  if (ConditionText.contains("<<") || ConditionText.contains(">>")) {
    // Shift operations can produce arbitrary values, not boolean
    // Even if there are boolean operators, shifts in the expression
    // make it more complex. For strict MISRA compliance, flag it.
    return false;
  }

  // If we found boolean operators, expression is OK
  if (HasBooleanOp)
    return true;

  // Check for relational operators < and >
  // Be careful not to match << or >> (already handled above)
  size_t Pos = 0;
  while ((Pos = ConditionText.find('<', Pos)) != StringRef::npos) {
    // Check it's not <<
    if (Pos + 1 >= ConditionText.size() || ConditionText[Pos + 1] != '<')
      return true;
    Pos += 2; // Skip both characters of <<
  }

  Pos = 0;
  while ((Pos = ConditionText.find('>', Pos)) != StringRef::npos) {
    // Check it's not >>
    if (Pos + 1 >= ConditionText.size() || ConditionText[Pos + 1] != '>')
      return true;
    Pos += 2; // Skip both characters of >>
  }

  // For single identifiers (macros), we cannot determine at this point
  // whether they expand to 0, 1, or other values without evaluating them.
  // The preprocessor has already evaluated them, but we don't have access
  // to the actual numeric value, only whether the result was true/false.
  //
  // According to MISRA, macros used directly should be OK if they evaluate
  // to 0 or 1. However, without macro expansion information, we'll be
  // conservative and allow single identifier usage.
  //
  // Check if it's a simple identifier (no operators)
  bool HasOperator = false;
  for (char C : ConditionText) {
    if (C == '+' || C == '-' || C == '*' || C == '/' || C == '%' || C == '&' ||
        C == '|' || C == '^' || C == '~' || C == '(' || C == ')') {
      HasOperator = true;
      break;
    }
  }

  // If it's a simple identifier without operators, allow it
  // This covers cases like: #if FEATURE_ENABLED
  if (!HasOperator) {
    // Check if it's a valid identifier-like expression
    bool AllIdentChars = true;
    for (char C : ConditionText) {
      if (!std::isalnum(C) && C != '_' && !std::isspace(C)) {
        AllIdentChars = false;
        break;
      }
    }
    if (AllIdentChars && !ConditionText.empty())
      return true; // Conservative: allow single macros
  }

  return false;
}

class IfExpressionValuePPCallbacks : public PPCallbacks {
public:
  IfExpressionValuePPCallbacks(ClangTidyCheck &Check, const SourceManager &SM,
                               Preprocessor *PP)
      : Check(Check), SM(SM), PP(PP) {}

  void If(SourceLocation Loc, SourceRange ConditionRange,
          ConditionValueKind ConditionValue) override {
    checkCondition(Loc, ConditionRange, "if");
  }

  void Elif(SourceLocation Loc, SourceRange ConditionRange,
            ConditionValueKind ConditionValue, SourceLocation IfLoc) override {
    checkCondition(Loc, ConditionRange, "elif");
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
  Preprocessor *PP;

  void checkCondition(SourceLocation Loc, SourceRange ConditionRange,
                      const char *DirectiveName) {
    if (Loc.isInvalid() || SM.isInSystemHeader(Loc))
      return;

    if (!isBooleanExpression(SM, PP, ConditionRange)) {
      Check.diag(Loc, "#%0 directive condition shall evaluate to 0 or 1")
          << DirectiveName;
    }
  }
};

} // anonymous namespace

void IfExpressionValueCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<IfExpressionValuePPCallbacks>(*this, SM, PP));
}

} // namespace clang::tidy::automotive

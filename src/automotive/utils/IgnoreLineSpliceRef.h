//===--- IgnoreLineSpliceRef.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IGNORELINESPLICEREF_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IGNORELINESPLICEREF_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

class IgnoreLineSpliceRefIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = char;
  using difference_type = std::ptrdiff_t;
  using pointer = char *;
  using reference = char &;

  IgnoreLineSpliceRefIterator(llvm::StringRef Text, size_t Index)
      : Text(Text), PrevIndex(Index), Index(Index) {
    skipLineSplice();
  }

  char operator*() const { return Text[Index]; }

  IgnoreLineSpliceRefIterator &operator++() {
    PrevIndex = Index;
    ++Index;
    skipLineSplice();
    return *this;
  }

  IgnoreLineSpliceRefIterator operator++(int) {
    IgnoreLineSpliceRefIterator temp = *this;
    ++(*this);
    return temp;
  }

  bool operator!=(const IgnoreLineSpliceRefIterator &Other) const {
    return Index != Other.Index;
  }

  size_t position() const { return Index; }

  size_t prevPosition() const { return PrevIndex; }

private:
  void skipLineSplice() {
    while (Index + 1 < Text.size() && Text[Index] == '\\' &&
           Text[Index + 1] == '\n') {
      Index += 2;
    }
  }

  llvm::StringRef Text;
  size_t PrevIndex;
  size_t Index;
};

class IgnoreLineSpliceRefReverseIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = char;
  using difference_type = std::ptrdiff_t;
  using pointer = char *;
  using reference = char &;

  IgnoreLineSpliceRefReverseIterator(llvm::StringRef Text, size_t Index)
      : Text(Text), Index(Index) {
    skipLineSplice();
  }

  char operator*() const { return Text[Index]; }

  IgnoreLineSpliceRefReverseIterator &operator--() {
    --Index;
    skipLineSplice();
    return *this;
  }

  IgnoreLineSpliceRefReverseIterator operator--(int) {
    IgnoreLineSpliceRefReverseIterator temp = *this;
    --(*this);
    return temp;
  }

  bool operator!=(const IgnoreLineSpliceRefReverseIterator &Other) const {
    return Index != Other.Index;
  }

  size_t position() const { return Index; }

private:
  void skipLineSplice() {
    while (Index > 0 && Text[Index] == '\\' && Text[Index - 1] == '\n') {
      Index -= 2;
    }
  }

  llvm::StringRef Text;
  size_t Index;
};

class IgnoreLineSpliceRef {
public:
  explicit IgnoreLineSpliceRef(llvm::StringRef Text) : Text(Text) {}

  IgnoreLineSpliceRefIterator begin() {
    return IgnoreLineSpliceRefIterator(Text, 0);
  }

  IgnoreLineSpliceRefIterator end() {
    return IgnoreLineSpliceRefIterator(Text, Text.size());
  }

  IgnoreLineSpliceRefReverseIterator rbegin() {
    return IgnoreLineSpliceRefReverseIterator(Text, Text.size() - 1);
  }

  IgnoreLineSpliceRefReverseIterator rend() {
    return IgnoreLineSpliceRefReverseIterator(Text, size_t(-1));
  }

private:
  llvm::StringRef Text;
};

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IGNORELINESPLICEREF_H

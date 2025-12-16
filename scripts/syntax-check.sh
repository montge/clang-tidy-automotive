#!/bin/bash
# Syntax check for automotive module source files
# Uses LLVM source tree headers (no full build required)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
source "${PROJECT_ROOT}/version.env"

cd "$PROJECT_ROOT"

# Include paths
INCLUDES=(
  "-I/usr/lib/llvm-20/include"
  "-I./${LLVM_DIR}/clang/include"
  "-I./${LLVM_DIR}/clang-tools-extra/clang-tidy"
  "-I./build/tools/clang/include"
  "-I./src"
)

# Find all C++ source files
FILES=$(find src/automotive -name '*.cpp' | sort)
TOTAL=$(echo "$FILES" | wc -l)
ERRORS=0
CHECKED=0

echo "Checking $TOTAL source files..."
echo ""

for file in $FILES; do
  CHECKED=$((CHECKED + 1))
  if ! clang++-20 -fsyntax-only -std=c++17 "${INCLUDES[@]}" "$file" 2>&1; then
    echo "FAIL: $file"
    ERRORS=$((ERRORS + 1))
  else
    printf "\r[%d/%d] %s" "$CHECKED" "$TOTAL" "$file"
  fi
done

echo ""
echo ""

if [ $ERRORS -gt 0 ]; then
  echo "FAILED: $ERRORS files with syntax errors"
  exit 1
else
  echo "PASSED: All $TOTAL files OK"
fi

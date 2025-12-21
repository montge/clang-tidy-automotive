#!/bin/bash

for file in test/checkers/automotive/function/*.c test/checkers/automotive/storage/*.c test/checkers/automotive/negative/*.c; do
  if [ -f "$file" ]; then
    check=$(grep -oP 'automotive-[a-z-]+' "$file" | head -1)
    if [ -n "$check" ]; then
      if ./build/bin/clang-tidy --checks="$check" --list-checks 2>&1 | grep -q "$check"; then
        echo "$file: CHECK EXISTS ($check)"
      else
        echo "$file: CHECK MISSING ($check)"
      fi
    fi
  fi
done

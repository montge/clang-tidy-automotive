#!/bin/bash

for test in avoid-function-parameter-modification function-edge-cases function-parameter-mismatch missing-static-inline no-implicit-func-decl; do
    # Extract check name from test file
    check=$(grep "RUN:.*automotive-" test/checkers/automotive/function/${test}.c | head -1 | sed 's/.*automotive-/automotive-/' | awk '{print $1}' | tr ',' '\n' | head -1)

    if [ -z "$check" ]; then
        echo "$test: NO RUN LINE FOUND"
        continue
    fi

    if ./build/bin/clang-tidy --checks="$check" --list-checks 2>&1 | grep -q "$check"; then
        echo "$test ($check): EXISTS"
    else
        echo "$test ($check): DOES NOT EXIST"
    fi
done

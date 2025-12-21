#!/bin/bash

for test in avoid-pointer-typedef duplicate-tag-name duplicate-typedef-name explicit-enumerator-values implicit-int; do
    check="automotive-$test"
    if ./build/bin/clang-tidy --checks="$check" --list-checks 2>&1 | grep -q "$check"; then
        echo "$test: EXISTS"
    else
        echo "$test: DOES NOT EXIST"
    fi
done

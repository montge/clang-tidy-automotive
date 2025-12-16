#!/usr/bin/bash
# Copy the prepatched files back to the prepatch directory to easier be able to commit
# Version is centralized in version.env

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/../version.env"

cp "../${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyForceLinker.h" .
cp "../${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidy.cpp" .
cp "../${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyOptions.h" .
cp "../${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyOptions.cpp" .
cp "../${LLVM_DIR}/clang-tools-extra/clang-tidy/CMakeLists.txt" .
cp "../${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.h" .
cp "../${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.cpp" .

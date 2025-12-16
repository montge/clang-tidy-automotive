#!/usr/bin/bash

# Create symbolic links from the clang-tidy-automotive source tree into LLVM.
# Version is centralized in version.env

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/version.env"

ln -f -s ../../../src/automotive/ "${LLVM_DIR}/clang-tools-extra/clang-tidy/"
ln -f -s ../../../../../test/checkers/automotive "${LLVM_DIR}/clang-tools-extra/test/clang-tidy/checkers/automotive"
ln -f -s ../../../src/ClangTidyDiagnosticMapping.cpp "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyDiagnosticMapping.cpp"
ln -f -s ../../../src/ClangTidyDiagnosticMapping.h "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyDiagnosticMapping.h"

# Create symlinks in the src directory.
directories=(
    "cert"
    "misc"
)

src_files=(
    "ClangTidy.h"
    "ClangTidyCheck.h"
    "ClangTidyDiagnosticConsumer.h"
    "ClangTidyModule.h"
    "ClangTidyModuleRegistry.h"
    "ClangTidyOptions.h"
    "ClangTidyProfiling.h"
    "ExpandModularHeadersPPCallbacks.h"
    "FileExtensionsSet.h"
    "GlobList.h"
    "NoLintDirectiveHandler.h"
)

for directory in "${directories[@]}"; do
    ln -f -s "../${LLVM_DIR}/clang-tools-extra/clang-tidy/${directory}" "src/${directory}"
done

for file in "${src_files[@]}"; do
    ln -f -s "../${LLVM_DIR}/clang-tools-extra/clang-tidy/${file}" "src/${file}"
done

# Copy the prepatched files to activate Automotive support
cp prepatch/ClangTidyForceLinker.h "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyForceLinker.h"
cp prepatch/ClangTidy.cpp "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidy.cpp"
cp prepatch/ClangTidyOptions.cpp "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyOptions.cpp"
cp prepatch/ClangTidyOptions.h "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyOptions.h"
cp prepatch/CMakeLists.txt "${LLVM_DIR}/clang-tools-extra/clang-tidy/CMakeLists.txt"
cp prepatch/ClangTidyDiagnosticConsumer.h "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.h"
cp prepatch/ClangTidyDiagnosticConsumer.cpp "${LLVM_DIR}/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.cpp"

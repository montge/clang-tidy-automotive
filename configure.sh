#!/usr/bin/bash

# Configure the build
# Version is centralized in version.env

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/version.env"

mkdir -p build
cd build
cmake -G Ninja \
      -DCMAKE_CXX_COMPILER=/usr/bin/clang++-20 \
      -DCMAKE_C_COMPILER=/usr/bin/clang-20 \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DCMAKE_BUILD_TYPE=Release \
      -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
      -DLLVM_TARGETS_TO_BUILD="X86" \
      -DLLVM_USE_LINKER=lld-20 \
      -DLLVM_PARALLEL_LINK_JOBS=2 \
      -DLLVM_USE_SPLIT_DWARF=ON \
      "../${LLVM_DIR}/llvm"
cd ..

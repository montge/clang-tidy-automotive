#!/usr/bin/bash
# Standalone build entry point — builds clang-tidy-automotive as an
# out-of-tree clang-tidy plugin against pre-installed LLVM/Clang 20 dev
# packages.
#
# Produces:
#   build-standalone/src/automotive/clangtidyautomotive.so   (the plugin)
#   build-standalone/clang-tidy/v20/clang-tidy-standalone    (vendored tool)
#
# Usage (host):  ./build-standalone.sh
# Usage (Docker, recommended on macOS):
#   docker build -t cta-build docker/
#   docker run --rm -v "$(pwd):/workspace" -w /workspace cta-build ./build-standalone.sh
#
# Use the plugin with the system clang-tidy-20:
#   clang-tidy-20 -load=./build-standalone/src/automotive/clangtidyautomotive.so \
#                 --checks='-*,automotive-*' --list-checks
#
# (The vendored clang-tidy-standalone tool has a separate ModuleRegistry
# from the system clang-tidy-20 and cannot currently load the plugin —
# use clang-tidy-20 instead.)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

BUILD_DIR="${BUILD_DIR:-build-standalone}"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

cmake -G Ninja \
  -DCMAKE_C_COMPILER="${CC:-/usr/bin/clang-20}" \
  -DCMAKE_CXX_COMPILER="${CXX:-/usr/bin/clang++-20}" \
  -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:-Release}" \
  -DCT_AUTOMOTIVE_ENABLE_DOCS=OFF \
  -DLLVM_DIR="${LLVM_DIR:-/usr/lib/llvm-20/cmake}" \
  -DClang_DIR="${Clang_DIR:-/usr/lib/llvm-20/lib/cmake/clang}" \
  ..

ninja

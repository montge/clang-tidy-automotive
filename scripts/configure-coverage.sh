#!/bin/bash
# Configure Build with Coverage Instrumentation
# This script configures the CMake build with coverage flags enabled
#
# Usage: ./scripts/configure-coverage.sh
#
# Prerequisites:
#   - LLVM source downloaded (run ./download.sh first)
#   - Symlinks set up (run ./setup.sh first)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${PROJECT_ROOT}/build"

# Default compiler version
LLVM_VERSION="${LLVM_VERSION:-20}"
CC="${CC:-clang-${LLVM_VERSION}}"
CXX="${CXX:-clang++-${LLVM_VERSION}}"

# LLVM source directory
LLVM_SOURCE="${PROJECT_ROOT}/llvm-project-llvmorg-20.1.8"

echo "================================"
echo " Configure Coverage Build"
echo "================================"
echo ""
echo "Build directory: ${BUILD_DIR}"
echo "LLVM source: ${LLVM_SOURCE}"
echo "C Compiler: ${CC}"
echo "C++ Compiler: ${CXX}"
echo ""

# Check prerequisites
if [ ! -d "$LLVM_SOURCE" ]; then
    echo "Error: LLVM source not found at $LLVM_SOURCE"
    echo "Run ./download.sh first"
    exit 1
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with coverage flags
cmake -G Ninja \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="-fprofile-instr-generate -fcoverage-mapping" \
    -DCMAKE_C_FLAGS="-fprofile-instr-generate -fcoverage-mapping" \
    -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
    -DLLVM_TARGETS_TO_BUILD="X86" \
    "${LLVM_SOURCE}/llvm"

echo ""
echo "Configuration complete!"
echo ""
echo "Next steps:"
echo "  1. Build clang-tidy: ninja -C build clang-tidy"
echo "  2. Run coverage: ./scripts/coverage.sh --report"

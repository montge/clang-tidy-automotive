#!/usr/bin/bash

# Download and extract LLVM
# Version is centralized in version.env

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/version.env"

wget "${LLVM_URL}"
tar xzvf "${LLVM_TARBALL}"

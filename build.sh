#!/usr/bin/bash
cd build
# Limit to 4 parallel jobs to reduce memory usage (adjust as needed)
# Each LLVM compile can use 1-2GB RAM, linking can use 10GB+
ninja -j2 clang-tidy clang-format
cd ..

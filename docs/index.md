---
layout: default
title: Home
nav_order: 1
description: "MISRA C:2025 compliance checks for clang-tidy"
permalink: /
---

# Clang-Tidy Automotive
{: .fs-9 }

MISRA C:2025 compliance checks for safety-critical automotive software development.
{: .fs-6 .fw-300 }

[Get Started](#getting-started){: .btn .btn-primary .fs-5 .mb-4 .mb-md-0 .mr-2 }
[View on GitHub](https://github.com/yourorg/clang-tidy-automotive){: .btn .fs-5 .mb-4 .mb-md-0 }

---

## Overview

Clang-Tidy Automotive provides **56+ static analysis checks** for MISRA C:2025 compliance, integrated with the LLVM/Clang toolchain.

### Key Features

| Feature | Description |
|:--------|:------------|
| **MISRA Coverage** | ~25% of MISRA C:2025 rules implemented |
| **Multi-Version Support** | Works with Clang 17, 18, 19, and 20 |
| **SonarQube Integration** | Export findings for compliance reporting |
| **Full Traceability** | Rule → Check → Test mapping |

### Quick Example

```bash
# Run all automotive checks
clang-tidy -checks="automotive-*" your_file.c --

# Run specific category
clang-tidy -checks="automotive-avoid-*" your_file.c --
```

---

## Getting Started

### Prerequisites

- Clang/LLVM 17-20
- CMake 3.20+
- Ninja (recommended)

### Installation

```bash
# Clone repository
git clone https://github.com/yourorg/clang-tidy-automotive.git
cd clang-tidy-automotive

# Download LLVM source and build
./download.sh
./setup.sh
mkdir build && cd build
cmake -G Ninja \
    -DCMAKE_CXX_COMPILER=clang++-20 \
    -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
    ../llvm-project-llvmorg-20.1.8/llvm
ninja clang-tidy
```

### Configuration

Create a `.clang-tidy` file in your project:

```yaml
Checks: >
  automotive-*,
  -automotive-avoid-goto

WarningsAsErrors: 'automotive-*'
```

---

## Coverage Summary

{: .note }
Currently implementing ~25% of MISRA C:2025 rules. Priority is on Required rules.

| Category | Coverage |
|:---------|:---------|
| Comments (3.x) | ██████████ 100% |
| Types (6.x) | ██████████ 100% |
| Overlapping (19.x) | ██████████ 100% |
| Control Flow (14.x) | ███████░░░ 75% |
| Statements (15.x) | ████░░░░░░ 43% |
| Standard Library (21.x) | ███░░░░░░░ 29% |

[View Full Coverage →]({% link misra-coverage.md %})

---

## Contributing

We welcome contributions! See our [contributing guide](https://github.com/yourorg/clang-tidy-automotive/blob/main/CONTRIBUTING.md) for details.

### About the Project

Clang-Tidy Automotive is distributed under the [Apache 2.0 License](https://github.com/yourorg/clang-tidy-automotive/blob/main/LICENSE).

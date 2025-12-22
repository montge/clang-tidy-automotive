# Clang-Tidy Automotive

A comprehensive collection of clang-tidy checks for automotive safety-critical code, implementing MISRA C and MISRA C++ coding guidelines across multiple standard versions.

[![SonarCloud](https://sonarcloud.io/api/project_badges/measure?project=montge_clang-tidy-automotive&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=montge_clang-tidy-automotive)
[![CI](https://github.com/montge/clang-tidy-automotive/actions/workflows/ci.yml/badge.svg)](https://github.com/montge/clang-tidy-automotive/actions/workflows/ci.yml)

## Features

- **172 custom checks** covering statements, expressions, types, functions, and more
- **Multi-version MISRA support:**
  - MISRA C:2012/2023 (65 checks)
  - MISRA C:2025 (8 checks)
  - MISRA C++:2023 (46 checks)
  - Generic automotive checks (53 checks)
- **SonarQube/SonarCloud integration** for enterprise quality gates
- **Comprehensive examples** with violation/compliant pairs
- **Doxygen documentation** for all check implementations

## Supported MISRA Standards

### MISRA C:2012/2023 (`automotive-c23-*`)

65 checks covering the core MISRA C guidelines:
- Essential type model and conversions
- Pointer safety and NULL handling
- Control flow and statement structure
- Standard library restrictions

### MISRA C:2025 (`automotive-c25-*`)

8 checks for the latest MISRA C:2025 standard:
- Well-formed switch statements (Rule 16.1)
- Else-if termination (Rule 15.7)
- For loop structure (Rule 14.2)
- Array initialization (Rule 9.7)
- Chained designators (Rule 9.6)

### MISRA C++:2023 (`automotive-cpp23-*`)

46 checks for modern C++ safety:
- Cast restrictions (C-style, const, reinterpret, dynamic)
- Object lifecycle (slicing, Rule of Five, virtual destructors)
- Exception safety (noexcept, throwing destructors)
- Lambda captures and conversions
- Template constraints and concepts
- Memory management restrictions

### Generic Checks (`automotive-*`)

53 checks applicable across standards:
- goto restrictions and label management
- Switch statement structure
- Compound statement requirements
- Preprocessor hygiene
- Unused code detection

See [MISRA Rule Inventory](docs/MISRA-RULE-INVENTORY.md) for C rules and [MISRA C++:2023 Inventory](docs/MISRA-CPP-2023-INVENTORY.md) for C++ rules.

## Getting Started

To build clang-tidy Automotive, follow these steps:

### Prerequisites

Clang 20 (or compatible version)

### Setup Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/PeterHallonmark/clang-tidy-automotive.git
   cd clang-tidy-automotive
   ```

2. Run the following setup scripts to build Clang-Tidy Automotive:
   ```bash
   ./download.sh
   ./setup.sh
   ./configure.sh
   ./build.sh
   ```

**Note:** The scripts will need to be adjusted to use a Docker container in
the future. This is part of the TODO list.

## Usage

To use the Automotive support in clang-tidy, enable the rules automotive-*
when using clang-tidy.

### C Code (MISRA C:2025)

```bash
# Run a specific check
./build/bin/clang-tidy --checks="automotive-avoid-goto" test/checkers/automotive/statement/avoid-goto.c --

# Run all automotive C checks
./build/bin/clang-tidy --checks="-*,automotive-*" your-code.c -- -std=c11
```

### C++ Code (MISRA C++:2023)

```bash
# Run C++ specific checks
./build/bin/clang-tidy --checks="automotive-cpp23-*" your-code.cpp -- -std=c++17

# Run all automotive checks (C and C++)
./build/bin/clang-tidy --checks="-*,automotive-*" your-code.cpp -- -std=c++17
```

### Sample C++ Checks

| Check ID | MISRA Rule | Description |
|----------|------------|-------------|
| `automotive-cpp23-req-8.2.1` | 8.2.1 | Avoid C-style casts |
| `automotive-cpp23-req-8.2.3` | 8.2.3 | Avoid const_cast |
| `automotive-cpp23-req-8.2.4` | 8.2.4 | Avoid reinterpret_cast |
| `automotive-cpp23-req-15.0.1` | 15.0.1 | Rule of Five |
| `automotive-cpp23-req-15.1.3` | 15.1.3 | Explicit constructors |
| `automotive-cpp23-req-15.3` | 15.3 | Avoid object slicing |
| `automotive-cpp23-req-15.7` | 15.7 | Virtual destructor requirement |
| `automotive-cpp23-req-18.4.1` | 18.4.1 | No throwing destructors |
| `automotive-cpp23-req-18.4.2` | 18.4.2 | Noexcept move operations |
| `automotive-cpp23-adv-6.8` | 6.8 | Suboptimal break/continue |
| `automotive-cpp23-req-7.4` | 7.4 | Assignment result not used |

Run `clang-tidy --list-checks --checks='automotive-cpp23-*'` to see all 46 C++ checks.

See [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) for more information.

## Documentation

- [MISRA C:2025 Rule Inventory](docs/MISRA-RULE-INVENTORY.md) - Complete C rule mapping
- [MISRA C++:2023 Rule Inventory](docs/MISRA-CPP-2023-INVENTORY.md) - Complete C++ rule mapping
- [SonarQube Integration](docs/sonarqube-integration.md) - CI/CD setup guide
- [Code Coverage](docs/CODE-COVERAGE.md) - Local coverage testing guide
- [Manual Review Directives](docs/MANUAL-REVIEW-DIRECTIVES.md) - Guidelines for manual review
- [Naming Convention](NAMING-CONVENTION.md) - Check class naming standards
- [API Documentation](docs/api/html/index.html) - Doxygen-generated docs

## Examples

The `examples/` directory contains violation/compliant code pairs:

```
examples/
├── directives/    # Dir 1.x - 5.x examples
└── rules/         # Rule 1.x - 23.x examples
```

Run checks on examples:
```bash
./build/bin/clang-tidy --checks="-*,automotive-*" examples/rules/rule-15/15.1-goto-violation.c --
```

## License

The repository follows LLVM's license structure to make it easier to upstream
changes to the LLVM project if and when that happens. :)

## Design note: diagnostic mapping layer

A significant part of this project right now is the diagnostic mapping layer.

The purpose of the mapping layer is to strictly separate rule content from
the clang-tidy checks themselves. This separation is essential in order to:

- avoid embedding copyrighted MISRA rule text in the codebase
- allow different interpretations or future revisions of MISRA rules without
  rewriting the checks
- Map native clang diagnostics towards MISRA rules
- make the design acceptable for potential upstreaming in LLVM

The mapping layer is still under active development. Once it is complete and
proven, an RFC will be written and proposed to LLVM to evaluate whether this
architecture could be upstreamed.

For background and context, see the
[LLVM discussion](https://discourse.llvm.org/t/will-clang-frontend-plan-accept-misra-check-tools/84754/33?u=peterhallonmark).

## Future Improvements

Move setup scripts to a Docker container for easier setup and environment
management.

## Contributing

Feel free to open issues or pull requests if you'd like to contribute to
this project. Contributions are always welcome!

**Note:** This project is not officially associated with MISRA or LLVM.
Any contributions or code changes are not officially endorsed by MISRA or
LLVM.

# Clang-Tidy Automotive

A collection of clang-tidy checks for automotive safety-critical code, supporting both MISRA C:2025 and MISRA C++:2023 guidelines.

[![SonarCloud](https://sonarcloud.io/api/project_badges/measure?project=montge_clang-tidy-automotive&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=montge_clang-tidy-automotive)
[![CI](https://github.com/montge/clang-tidy-automotive/actions/workflows/ci.yml/badge.svg)](https://github.com/montge/clang-tidy-automotive/actions/workflows/ci.yml)

## Features

- **101 custom checks** covering statements, expressions, types, functions, and more
- **MISRA C:2025 alignment** with documented rule mappings (93/176 rules)
- **MISRA C++:2023 support** with dedicated C++ checks (3 checks, ~75 via clang-tidy aliases)
- **SonarQube/SonarCloud integration** for enterprise quality gates
- **Comprehensive examples** with violation/compliant pairs
- **Doxygen documentation** for all check implementations

## Check Categories

### MISRA C:2025 Checks

| Category | Checks | Coverage |
|----------|--------|----------|
| Statement | 15 | goto, switch, compound statements, control flow |
| Expression | 8 | assignments, conditions, side effects |
| Function | 6 | prototypes, return values, parameters |
| Type | 7 | unions, enums, implicit types, duplicates |
| Bitfield | 3 | types, signed single-bit |
| Literal | 3 | octal, suffixes, escapes |
| Preprocessor | 8 | macros, #undef, hash operators, includes |
| Pointer | 6 | NULL, void pointers, arithmetic, casts |
| Array | 5 | VLA, flexible members, initialization |
| Standard Library | 7 | dangerous functions |
| Storage | 3 | linkage, static |

### MISRA C++:2023 Checks

| Category | Checks | Coverage |
|----------|--------|----------|
| cpp23 | 3 | C-style casts, object slicing, virtual destructors |

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

### Available C++ Checks

| Check ID | MISRA Rule | Description |
|----------|------------|-------------|
| `automotive-cpp23-req-8.2.1` | 8.2.1 | Avoid C-style casts |
| `automotive-cpp23-req-15.3` | 15.3 | Avoid object slicing |
| `automotive-cpp23-req-15.7` | 15.7 | Virtual destructor requirement |

See [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) for more information.

## Documentation

- [MISRA C:2025 Rule Inventory](docs/MISRA-RULE-INVENTORY.md) - Complete C rule mapping
- [MISRA C++:2023 Rule Inventory](docs/MISRA-CPP-2023-INVENTORY.md) - Complete C++ rule mapping
- [SonarQube Integration](docs/sonarqube-integration.md) - CI/CD setup guide
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

## Future Improvements

Move setup scripts to a Docker container for easier setup and environment
management.

## Contributing

Feel free to open issues or pull requests if you'd like to contribute to
this project. Contributions are always welcome!

**Note:** This project is not officially associated with MISRA or LLVM.
Any contributions or code changes are not officially endorsed by MISRA or 
LLVM.

# Clang-Tidy Automotive

A collection of clang-tidy checks for automotive safety-critical C code, inspired by MISRA C:2025/C:2012 guidelines.

[![SonarCloud](https://sonarcloud.io/api/project_badges/measure?project=montge_clang-tidy-automotive&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=montge_clang-tidy-automotive)
[![CI](https://github.com/montge/clang-tidy-automotive/actions/workflows/ci.yml/badge.svg)](https://github.com/montge/clang-tidy-automotive/actions/workflows/ci.yml)

## Features

- **55+ custom checks** covering statements, expressions, types, functions, and more
- **MISRA C:2025 alignment** with documented rule mappings
- **SonarQube/SonarCloud integration** for enterprise quality gates
- **Comprehensive examples** with violation/compliant pairs
- **Doxygen documentation** for all check implementations

## Check Categories

| Category | Checks | Coverage |
|----------|--------|----------|
| Statement | 12 | goto, switch, compound statements |
| Expression | 3 | assignments, conditions |
| Function | 6 | prototypes, return values |
| Type | 4 | unions, enums, implicit types |
| Bitfield | 3 | types, signed single-bit |
| Literal | 3 | octal, suffixes, escapes |
| Preprocessor | 6 | macros, #undef, hash operators |
| Pointer | 2 | NULL, void pointers |
| Array | 4 | VLA, flexible members |
| Standard Library | 7 | dangerous functions |

See [MISRA Rule Inventory](docs/MISRA-RULE-INVENTORY.md) for complete mapping.

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
when using clang-tidy. The example below assumes the avoid-goto.c test file 
and the root directory of clang-tidy-automotive.
   ```bash
   ./build/bin/clang-tidy --checks="automotive-avoid-goto" test/checkers/automotive/statement/avoid-goto.c -- 
   ```

See [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) for more information.

## Documentation

- [MISRA Rule Inventory](docs/MISRA-RULE-INVENTORY.md) - Complete rule mapping
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

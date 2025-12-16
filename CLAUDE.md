# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Clang-Tidy Automotive is a collection of clang-tidy checks inspired by automotive safety coding standards (MISRA C:2023/C:2012). The checks are implemented as an LLVM clang-tidy module that integrates into the LLVM build system.

## Build Commands

```bash
# Initial setup (run once)
./download.sh      # Downloads LLVM source (version defined in version.env)
./setup.sh         # Creates symlinks into LLVM tree and copies prepatched files
./configure.sh     # Runs CMake with Ninja generator

# Build
./build.sh         # Builds clang-tidy and clang-format via ninja

# Format code
./clang-format.sh  # Formats all .cpp and .h files in src/
```

## Running Checks

```bash
# Run a specific check on a test file
./build/bin/clang-tidy --checks="automotive-avoid-goto" test/checkers/automotive/statement/avoid-goto.c --

# Run with config file
./build/bin/clang-tidy test.c -config-file=./.clang-tidy --
```

## Architecture

### Integration with LLVM
The project symlinks its source code into an LLVM source tree (directory name defined in `version.env`). Key integration points:
- `src/automotive/` → symlinked into `clang-tools-extra/clang-tidy/automotive/`
- `test/checkers/automotive/` → symlinked into `clang-tools-extra/test/clang-tidy/checkers/automotive/`
- `prepatch/` contains modified LLVM files that enable the automotive module

### Module Structure
- `src/automotive/AutomotiveTidyModule.cpp` - Main module registration, delegates to component factories
- Components organized by domain: `array/`, `bitfield/`, `comment/`, `expression/`, `function/`, `literal/`, `operator/`, `pointer/`, `preprocessor/`, `return/`, `statement/`, `stdlib/`, `storage/`, `type/`, `unused-code/`
- Each component has a `*Component.cpp` that registers its checks via `addCheckFactories()`

### Project Structure
```
clang-tidy-automotive/
├── src/automotive/          # Check implementations (55+ checks)
├── test/checkers/automotive/ # Test files for each check
├── examples/                # Violation/compliant example pairs
│   ├── directives/         # Dir 1.x - 5.x examples
│   └── rules/              # Rule 1.x - 23.x examples
├── docs/                    # Documentation
│   ├── MISRA-RULE-INVENTORY.md
│   ├── sonarqube-integration.md
│   └── MANUAL-REVIEW-DIRECTIVES.md
├── scripts/                 # Utility scripts
│   ├── clang-tidy-to-sarif.py
│   ├── clang-tidy-to-sonarqube.py
│   └── misra-compliance-report.py
├── config/                  # Configuration files
│   └── misra-rule-mapping.json
└── .github/workflows/       # CI/CD pipelines
```

### Adding a New Check
1. Create `YourCheck.h` and `YourCheck.cpp` in the appropriate component directory
2. Register in the component's `*Component.cpp` file
3. Add source files to the component's `CMakeLists.txt`
4. Add test file in `test/checkers/automotive/`

### Naming Convention
Check classes use descriptive prefixes (see `NAMING-CONVENTION.md`):
- **Avoid**: Something that should not be done (e.g., `AvoidGotoCheck`)
- **Missing**: Something important is absent (e.g., `MissingDefaultInSwitchStmtCheck`)
- **Wrong**: Something violates a constraint (e.g., `WrongBitfieldTypeCheck`)
- **Unstructured**: Layout/structure issues

Rule IDs are only used during registration, not in class names:
```cpp
CheckFactories.registerCheck<AvoidGotoCheck>("automotive-avoid-goto");
```

<!-- OPENSPEC:START -->
# OpenSpec Instructions

These instructions are for AI assistants working in this project.

Always open `@/openspec/AGENTS.md` when the request:
- Mentions planning or proposals (words like proposal, spec, change, plan)
- Introduces new capabilities, breaking changes, architecture shifts, or big performance/security work
- Sounds ambiguous and you need the authoritative spec before coding

Use `@/openspec/AGENTS.md` to learn:
- How to create and apply change proposals
- Spec format and conventions
- Project structure and guidelines

Keep this managed block so 'openspec update' can refresh the instructions.

<!-- OPENSPEC:END -->
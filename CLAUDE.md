# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Clang-Tidy Automotive is a collection of clang-tidy checks inspired by automotive safety coding standards (MISRA C:2023/C:2012). The checks are implemented as an LLVM clang-tidy module that integrates into the LLVM build system.

## Build Commands

The project supports two build modes. Standalone is the default going forward.

### Standalone build (recommended)

Builds the automotive checks as an out-of-tree clang-tidy plugin against
pre-installed LLVM 20 dev packages. Much faster than building all of LLVM.

```bash
# Native Linux (Debian/Ubuntu with llvm-20-dev, libclang-20-dev installed)
./build-standalone.sh

# Docker (recommended on macOS — no native arm64 image, runs under emulation)
docker build -t cta-build docker/
docker run --rm -v "$(pwd):/workspace" -w /workspace cta-build ./build-standalone.sh
```

Produces:
- `build-standalone/src/automotive/clangtidyautomotive.so` — the plugin
- `build-standalone/clang-tidy/v20/clang-tidy-standalone` — vendored clang-tidy

### In-tree build (legacy)

Builds the full LLVM source tree with our checks linked in. Hours-long build.

```bash
./download.sh      # Downloads LLVM 20.1.8 source (~220MB tarball)
./setup.sh         # Symlinks src/automotive into LLVM tree, copies prepatch files
./configure.sh     # Runs CMake (Linux paths — requires apt-installed clang-20, lld-20)
./build.sh         # Builds clang-tidy via ninja
```

### Format code

```bash
./clang-format.sh  # Formats all .cpp and .h files in src/
```

## Running Checks

### With standalone plugin (via system clang-tidy-20)

```bash
# List all automotive checks
clang-tidy-20 -load=./build-standalone/src/automotive/clangtidyautomotive.so \
              --checks='-*,automotive-*' --list-checks

# Run a specific check on a test file
clang-tidy-20 -load=./build-standalone/src/automotive/clangtidyautomotive.so \
              --checks='-*,automotive-avoid-goto' \
              test/checkers/automotive/statement/avoid-goto.c --
```

Note: the vendored `clang-tidy-standalone` binary has its own `ModuleRegistry`
that's separate from the system clang-tidy's. The plugin currently only loads
into the system `clang-tidy-20` (or any clang-tidy linked against the same
`libclang-cpp.so.20` as the plugin).

### With in-tree build

```bash
./build/bin/clang-tidy --checks="automotive-avoid-goto" \
  test/checkers/automotive/statement/avoid-goto.c --
```

## Architecture

The project supports two build modes via the `add_automotive_component()`
macro in [cmake/AutomotiveBuildMode.cmake](src/automotive/cmake/AutomotiveBuildMode.cmake):

- **Standalone (out-of-tree)**: each component is a static library with PIC,
  linked into a single `clangtidyautomotive.so` MODULE. Driven by the top-level
  [CMakeLists.txt](CMakeLists.txt) which uses `find_package(LLVM)`/`find_package(Clang)`
  and pulls in [cmake/ClangTidyOutOfTree.cmake](cmake/ClangTidyOutOfTree.cmake)
  to provide shims for `add_clang_library` and friends. The vendored
  clang-tidy sources live in [clang-tidy/v20/](clang-tidy/v20/).

- **In-tree (legacy)**: each component is an LLVM `add_clang_library` STATIC
  target. Activated when `AUTOMOTIVE_IN_TREE=ON` (the default if symlinked
  into a `llvm-project` checkout). Uses [prepatch/](prepatch/) files copied
  over the LLVM source by [setup.sh](setup.sh).

### Integration with LLVM (in-tree mode only)
The legacy in-tree mode symlinks the source into an LLVM source tree (directory name defined in `version.env`). Key integration points:
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
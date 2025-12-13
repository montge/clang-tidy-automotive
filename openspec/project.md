# Project Context

## Purpose
Clang-Tidy Automotive provides static analysis checks inspired by MISRA C:2023/2025 and MISRA C++:2023 coding standards for automotive safety-critical software development. The project extends LLVM's clang-tidy with an `automotive` module containing safety-focused checks.

## Tech Stack
- C++17 (LLVM coding style)
- LLVM/Clang 20.x
- CMake + Ninja build system
- clang-tidy AST matchers and PPCallbacks for checks

## Project Conventions

### Code Style
- Follow LLVM coding conventions
- Use clang-format with LLVM style
- Check class naming: `{Prefix}{Description}Check` (e.g., `AvoidGotoCheck`, `MissingDefaultInSwitchStmtCheck`)
- Prefixes: `Avoid`, `Missing`, `Wrong`, `Unstructured`
- Registration IDs: `automotive-{descriptive-name}` or `automotive-c23-{category}-{rule}`

### Architecture Patterns
- Checks organized by domain component (array, bitfield, comment, expression, function, etc.)
- Each component has a `*Component.cpp` that registers checks via `addCheckFactories()`
- Main module (`AutomotiveTidyModule.cpp`) delegates to component factories
- Integration with LLVM via symlinks into `llvm-project-llvmorg-20.1.8/`

### Testing Strategy
- Test files in `test/checkers/automotive/` organized by component
- Each test file demonstrates both violations and compliant code
- Target: 90%+ code coverage
- Use LLVM lit testing framework

### Git Workflow
- Main branch: `main`
- Feature branches for development
- Commit messages: concise description of changes

## Domain Context
- MISRA (Motor Industry Software Reliability Association) standards define coding guidelines for safety-critical automotive software
- Rules are categorized as: Required (Req), Advisory (Adv), Mandatory (Man)
- Directives (Dir) are guidelines requiring judgment; Rules are specific constraints
- MISRA-C:2025 has ~181 rules and ~20 directives
- Checks must not reproduce copyrighted MISRA rule text

## Important Constraints
- Cannot include MISRA rule text (copyright protected)
- Must work with LLVM 20.x build system
- Checks should be usable standalone or with SonarQube integration
- Performance critical: checks run on every compilation

## External Dependencies
- LLVM/Clang 20.1.8 source tree
- Private MISRA-Docs repository (licensed, not redistributable)
- SonarQube (target integration for compliance reporting)

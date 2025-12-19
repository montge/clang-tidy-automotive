# MISRA C++:2023 Rule Inventory

This document provides a comprehensive mapping between MISRA C++:2023 rules and their implementation status in clang-tidy-automotive.

## Overview

MISRA C++:2023 (Guidelines for the use of the C++ language in critical systems) is the successor to MISRA C++:2008 and incorporates elements from AUTOSAR C++14. It targets C++17 and includes approximately 180 rules organized into categories.

## Legend

| Status | Description |
|--------|-------------|
| **Implemented** | Custom check in clang-tidy-automotive |
| **Clang Built-in** | Covered by existing clang/clang-tidy checks |
| **Planned** | Scheduled for implementation |
| **Manual** | Requires manual code review |
| **N/A** | Not applicable to static analysis |

## Rule Categories

MISRA C++:2023 rules are organized into these categories:

1. **Standard Conventions** (Rules 0.x) - Documentation requirements
2. **Lexical Conventions** (Rules 2.x-3.x) - Characters, literals, identifiers
3. **Basic Concepts** (Rules 4.x-5.x) - Object lifetime, types
4. **Expressions** (Rules 6.x-8.x) - Operators, conversions, expressions
5. **Statements** (Rules 9.x) - Control flow
6. **Declarations** (Rules 10.x-11.x) - Functions, variables
7. **Classes** (Rules 12.x-15.x) - Constructors, inheritance
8. **Exception Handling** (Rules 18.x) - try/catch/throw
9. **Templates** (Rules 17.x) - Template usage
10. **Preprocessing Directives** (Rules 19.x) - Macros
11. **Standard Library** (Rules 21.x-28.x) - STL usage

## Summary Statistics

| Category | Total | Implemented | Clang | Planned | Manual |
|----------|-------|-------------|-------|---------|--------|
| Standard Conventions | 5 | 0 | 0 | 0 | 5 |
| Lexical Conventions | 20 | 5 | 8 | 2 | 5 |
| Basic Concepts | 15 | 2 | 5 | 3 | 5 |
| Expressions | 35 | 18 | 10 | 2 | 5 |
| Statements | 15 | 10 | 3 | 1 | 1 |
| Declarations | 25 | 8 | 12 | 3 | 2 |
| Classes | 30 | 8 | 15 | 2 | 5 |
| Exception Handling | 10 | 4 | 5 | 0 | 1 |
| Templates | 15 | 0 | 5 | 5 | 5 |
| Preprocessing | 10 | 6 | 2 | 1 | 1 |
| Standard Library | 20 | 3 | 10 | 5 | 2 |
| **Total** | **~200** | **~64** | **~75** | **~24** | **~37** |

*Note: Statistics are estimates based on MISRA C++:2023 public summaries. 9 automotive-cpp23 checks implemented.*

## Rules Shared with MISRA C:2025

Many rules are common between MISRA C:2025 and MISRA C++:2023. The following automotive checks apply to both:

| Check ID | MISRA C Rule | MISRA C++ Rule | Description |
|----------|--------------|----------------|-------------|
| `automotive-avoid-goto` | 15.1 | 9.6.4 | No goto statements |
| `automotive-avoid-comma-operator` | 12.3 | 8.19.1 | No comma operator |
| `automotive-avoid-octal-number` | 7.1 | 5.13.2 | No octal literals |
| `automotive-avoid-trigraph` | 4.2 | 5.2.1 | No trigraphs |
| `automotive-missing-default-in-switch` | 16.4 | 9.4.2 | Default in switch |
| `automotive-missing-break-in-case` | 16.3 | 9.4.1 | No fallthrough |
| `automotive-missing-compound` | 15.6 | 9.3.1 | Compound statements |
| `automotive-avoid-macro-identifier-conflict` | 5.4 | 19.0.1 | Macro naming |
| `automotive-avoid-reserved-macro-identifier` | 21.1 | 19.0.4 | Reserved identifiers |
| `automotive-missing-header-guard` | Dir 4.10 | 19.3.1 | Header guards |

## Existing Clang-Tidy Check Mappings

### bugprone-* (Bug-Prone Patterns)

| Clang-Tidy Check | MISRA C++ Rule | Description |
|------------------|----------------|-------------|
| `bugprone-branch-clone` | 0.4.1 | Duplicated code detection |
| `bugprone-empty-catch` | 18.3.2 | Empty exception handlers |
| `bugprone-exception-escape` | 18.5.1 | Exception leakage |
| `bugprone-forwarding-reference-overload` | 17.8.1 | Perfect forwarding issues |
| `bugprone-infinite-loop` | 9.3.3 | Infinite loops |
| `bugprone-macro-parentheses` | 19.0.3 | Macro safety |
| `bugprone-move-forwarding-reference` | 17.8.2 | Move semantics |
| `bugprone-narrowing-conversions` | 8.4.1 | Narrowing conversions |
| `bugprone-reserved-identifier` | 5.10.1 | Reserved identifiers |
| `bugprone-sizeof-expression` | 8.6.2 | sizeof misuse |
| `bugprone-suspicious-enum-usage` | 10.2.3 | Enum type safety |
| `bugprone-throw-keyword-missing` | 18.1.1 | Missing throw |
| `bugprone-virtual-near-miss` | 13.3.1 | Virtual function override |

### cppcoreguidelines-* (C++ Core Guidelines)

| Clang-Tidy Check | MISRA C++ Rule | Description |
|------------------|----------------|-------------|
| `cppcoreguidelines-avoid-goto` | 9.6.4 | No goto |
| `cppcoreguidelines-c-copy-assignment-signature` | 15.8.1 | Copy assignment |
| `cppcoreguidelines-init-variables` | 11.6.1 | Initialize variables |
| `cppcoreguidelines-interfaces-global-init` | 6.9.2 | Global init order |
| `cppcoreguidelines-macro-usage` | 19.0.2 | Macro limitations |
| `cppcoreguidelines-narrowing-conversions` | 8.4.1 | Narrowing |
| `cppcoreguidelines-no-malloc` | 21.6.1 | No malloc in C++ |
| `cppcoreguidelines-prefer-member-initializer` | 15.4.1 | Member init lists |
| `cppcoreguidelines-pro-bounds-array-to-pointer-decay` | 8.1.1 | Array decay |
| `cppcoreguidelines-pro-bounds-constant-array-index` | 8.1.2 | Array bounds |
| `cppcoreguidelines-pro-bounds-pointer-arithmetic` | 8.1.3 | Pointer arithmetic |
| `cppcoreguidelines-pro-type-const-cast` | 8.2.3 | const_cast |
| `cppcoreguidelines-pro-type-cstyle-cast` | 8.2.1 | C-style cast |
| `cppcoreguidelines-pro-type-reinterpret-cast` | 8.2.2 | reinterpret_cast |
| `cppcoreguidelines-pro-type-union-access` | 12.3.1 | Union access |
| `cppcoreguidelines-slicing` | 15.3.1 | Object slicing |
| `cppcoreguidelines-special-member-functions` | 15.0.1 | Rule of five |
| `cppcoreguidelines-virtual-class-destructor` | 15.7.1 | Virtual destructors |

### modernize-* (C++ Modernization)

| Clang-Tidy Check | MISRA C++ Rule | Description |
|------------------|----------------|-------------|
| `modernize-avoid-bind` | 21.8.1 | Use lambdas |
| `modernize-avoid-c-arrays` | 11.3.1 | Use std::array |
| `modernize-deprecated-headers` | 21.1.1 | C++ headers |
| `modernize-loop-convert` | 9.3.2 | Range-based loops |
| `modernize-make-shared` | 21.5.1 | Smart pointers |
| `modernize-make-unique` | 21.5.1 | Smart pointers |
| `modernize-pass-by-value` | 15.8.2 | Move semantics |
| `modernize-raw-string-literal` | 5.13.5 | Raw strings |
| `modernize-redundant-void-arg` | 11.4.1 | void parameters |
| `modernize-replace-auto-ptr` | 21.5.2 | Deprecated types |
| `modernize-return-braced-init-list` | 11.6.2 | Brace init |
| `modernize-use-auto` | 10.1.2 | auto keyword |
| `modernize-use-bool-literals` | 5.13.7 | Boolean literals |
| `modernize-use-emplace` | 22.4.1 | Container emplacement |
| `modernize-use-equals-default` | 15.0.2 | Defaulted functions |
| `modernize-use-equals-delete` | 15.0.3 | Deleted functions |
| `modernize-use-nodiscard` | 10.6.1 | [[nodiscard]] |
| `modernize-use-noexcept` | 18.4.1 | noexcept |
| `modernize-use-nullptr` | 7.11.1 | nullptr |
| `modernize-use-override` | 13.3.2 | override |
| `modernize-use-using` | 10.1.3 | using vs typedef |

### hicpp-* (High Integrity C++)

| Clang-Tidy Check | MISRA C++ Rule | Description |
|------------------|----------------|-------------|
| `hicpp-avoid-goto` | 9.6.4 | No goto |
| `hicpp-braces-around-statements` | 9.3.1 | Compound statements |
| `hicpp-exception-baseclass` | 18.3.1 | Exception types |
| `hicpp-multiway-paths-covered` | 9.4.2 | Switch completeness |
| `hicpp-no-array-decay` | 8.1.1 | Array decay |
| `hicpp-no-assembler` | 10.4.1 | No asm |
| `hicpp-noexcept-move` | 18.4.2 | Move noexcept |
| `hicpp-signed-bitwise` | 8.8.1 | Signed bitwise |
| `hicpp-special-member-functions` | 15.0.1 | Rule of five |
| `hicpp-use-auto` | 10.1.2 | auto keyword |
| `hicpp-use-equals-default` | 15.0.2 | Defaulted functions |
| `hicpp-use-override` | 13.3.2 | override |

### cert-* (CERT C++ Secure Coding)

| Clang-Tidy Check | MISRA C++ Rule | Description |
|------------------|----------------|-------------|
| `cert-dcl50-cpp` | 11.1.1 | Variadic functions |
| `cert-dcl58-cpp` | 5.10.2 | std namespace |
| `cert-err33-c` | 0.1.1 | Error checking |
| `cert-err52-cpp` | 18.1.2 | setjmp/longjmp |
| `cert-err58-cpp` | 6.9.1 | Static init exceptions |
| `cert-err60-cpp` | 18.2.1 | Exception objects |
| `cert-msc50-cpp` | 26.5.1 | Random generation |
| `cert-oop57-cpp` | 12.0.1 | Trivial types |

## Implemented Automotive C++ Checks

The following MISRA C++:2023 rules have been implemented in clang-tidy-automotive:

| Check ID | MISRA Rule | Description | Source File |
|----------|------------|-------------|-------------|
| `automotive-cpp23-req-8.2.1` | 8.2.1 | C-style casts shall not be used | `cpp23/AvoidCStyleCastCheck.cpp` |
| `automotive-cpp23-adv-8.2.5` | 8.2.5 | dynamic_cast should not be used | `cpp23/AvoidDynamicCastCheck.cpp` |
| `automotive-cpp23-req-8.4.1` | 8.4.1 | Narrowing conversions shall not be used | `cpp23/AvoidNarrowingConversionCheck.cpp` |
| `automotive-cpp23-req-15.0.1` | 15.0.1 | Rule of Five for special member functions | `cpp23/RuleOfFiveCheck.cpp` |
| `automotive-cpp23-req-15.1.3` | 15.1.3 | Single-argument constructors shall be explicit | `cpp23/ExplicitConstructorCheck.cpp` |
| `automotive-cpp23-req-15.3` | 15.3 | Object slicing shall be avoided | `cpp23/AvoidSlicingCheck.cpp` |
| `automotive-cpp23-req-15.7` | 15.7 | Classes with virtual functions require virtual destructors | `cpp23/VirtualDestructorCheck.cpp` |
| `automotive-cpp23-req-18.4.1` | 18.4.1 | Destructors shall not throw exceptions | `cpp23/AvoidThrowingDestructorCheck.cpp` |
| `automotive-cpp23-req-18.4.2` | 18.4.2 | Move operations shall be noexcept | `cpp23/NoexceptMoveCheck.cpp` |

## Planned Automotive C++ Checks

The following MISRA C++:2023 rules are planned for custom implementation:

### Expression Checks (C++)
| Planned Check | MISRA Rule | Priority | Description |
|---------------|------------|----------|-------------|
| ~~`automotive-cpp23-avoid-narrowing`~~ | ~~8.4.1~~ | ~~High~~ | ~~Narrowing conversions~~ (Implemented as automotive-cpp23-req-8.4.1) |
| ~~`automotive-cpp23-avoid-dynamic-cast`~~ | ~~8.2.5~~ | ~~Medium~~ | ~~dynamic_cast limitations~~ (Implemented as automotive-cpp23-adv-8.2.5) |
| `automotive-cpp23-explicit-conversion` | 8.3.1 | Medium | Explicit conversions |

### Class Checks (C++)
| Planned Check | MISRA Rule | Priority | Description |
|---------------|------------|----------|-------------|
| ~~`automotive-cpp23-rule-of-five`~~ | ~~15.0.1~~ | ~~High~~ | ~~Special member functions~~ (Implemented as automotive-cpp23-req-15.0.1) |
| ~~`automotive-cpp23-explicit-ctor`~~ | ~~15.1.3~~ | ~~Medium~~ | ~~Explicit constructors~~ (Implemented as automotive-cpp23-req-15.1.3) |

### Exception Checks (C++)
| Planned Check | MISRA Rule | Priority | Description |
|---------------|------------|----------|-------------|
| ~~`automotive-cpp23-noexcept-move`~~ | ~~18.4.2~~ | ~~High~~ | ~~noexcept on move~~ (Implemented as automotive-cpp23-req-18.4.2) |
| ~~`automotive-cpp23-exception-spec`~~ | ~~18.4.1~~ | ~~Medium~~ | ~~Exception specifications~~ (Implemented as automotive-cpp23-req-18.4.1) |

### Template Checks (C++)
| Planned Check | MISRA Rule | Priority | Description |
|---------------|------------|----------|-------------|
| `automotive-cpp23-template-instantiation` | 17.0.1 | Medium | Template usage |
| `automotive-cpp23-concept-constraints` | 17.1.1 | Low | Concepts (C++20) |

## Manual Review Rules

The following rules require manual code review and cannot be fully automated:

1. **Documentation Rules (0.x)** - Comments, requirements traceability
2. **Design Rules** - Architecture decisions, naming conventions
3. **Complexity Rules** - Cyclomatic complexity, nesting depth
4. **Safety Analysis** - FMEA integration, safety impact

## Implementation Priorities

### Phase 1: High-Priority Rules (Q1)
- Rules with existing MISRA C equivalents
- Rules covered by cppcoreguidelines but need automotive ID aliasing
- Critical safety rules (exception handling, memory safety)

### Phase 2: Medium-Priority Rules (Q2)
- Modern C++ rules (C++17 features)
- Template-related rules
- STL usage rules

### Phase 3: Low-Priority Rules (Q3)
- Style rules
- Documentation rules
- Highly specialized rules

## References

- MISRA C++:2023 - Guidelines for the use of the C++ language in critical systems
- AUTOSAR C++14 Coding Guidelines
- C++ Core Guidelines (isocpp.github.io)
- CERT C++ Secure Coding Standard
- High Integrity C++ (HICPP)

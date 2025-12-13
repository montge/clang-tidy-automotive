# Manual Review Directives

This document lists MISRA C:2025 directives that cannot be fully verified by static analysis and require manual code review.

## Overview

MISRA directives describe high-level principles and guidelines that often require human judgment, documentation review, or runtime verification. Unlike rules, which have specific syntactic patterns that can be detected, directives typically address design decisions, process requirements, or behavioral properties.

## Directives Requiring Manual Review

### Dir 1.1 - Implementation-Defined Behavior (Required)

**What it requires:** All implementation-defined behavior shall be documented.

**Why static analysis cannot fully verify:**
- Requires review of documentation, not just code
- Implementation-defined behaviors vary by compiler/platform
- Need to verify documentation exists AND is accurate

**Manual review checklist:**
- [ ] Document all uses of implementation-defined behavior
- [ ] Specify target compiler and version
- [ ] Document expected behavior for each case
- [ ] Review shifts of signed values
- [ ] Review struct padding assumptions
- [ ] Review integer promotion behavior

### Dir 2.1 - Assembly Language (Required)

**What it requires:** All assembly language shall be encapsulated and isolated.

**Why static analysis cannot fully verify:**
- Assembly syntax varies by platform
- Encapsulation is a design pattern, not syntax
- Need to verify assembly is in dedicated files/modules

**Manual review checklist:**
- [ ] Assembly code is in separate source files
- [ ] Assembly has documented C interfaces
- [ ] No inline assembly in application logic
- [ ] Assembly files are clearly marked/named
- [ ] Platform dependencies are isolated

### Dir 3.1 - Compiler Warnings (Required)

**What it requires:** All code shall compile without any warnings with all warnings enabled.

**Why static analysis cannot fully verify:**
- Requires actual compilation with specific flags
- Warning behavior varies by compiler version
- CI/CD verification is more appropriate

**Manual review checklist:**
- [ ] Build system enables all warnings (-Wall -Wextra)
- [ ] No #pragma to disable warnings without justification
- [ ] CI pipeline fails on warnings
- [ ] Warnings-as-errors enabled in release builds

### Dir 4.1 - Run-Time Failures (Required)

**What it requires:** Run-time failures shall be minimized.

**Why static analysis cannot fully verify:**
- Requires runtime testing and analysis
- Depends on input data and execution paths
- Some failures only detectable at runtime

**Manual review checklist:**
- [ ] Division by zero protected
- [ ] Array bounds checked
- [ ] Pointer validity checked before use
- [ ] Arithmetic overflow handled
- [ ] Stack overflow mitigations in place
- [ ] Memory allocation failures handled

### Dir 4.3 - Assembly Encapsulation (Required)

**What it requires:** Assembly language shall be encapsulated.

**Why static analysis cannot fully verify:**
- Similar to Dir 2.1
- Encapsulation is architectural
- Requires design review

**Manual review checklist:**
- [ ] Assembly wrapped in C functions
- [ ] Clear API boundaries defined
- [ ] No assembly in headers
- [ ] Portable C alternatives documented

### Dir 4.4 - Defensive Programming (Advisory)

**What it requires:** Sections of code shall not be "commented out".

**Why static analysis has limitations:**
- Difficult to distinguish comments explaining code from commented-out code
- Context-dependent determination
- May flag legitimate documentation

**Manual review checklist:**
- [ ] No blocks of code in comments
- [ ] Use #if 0 with justification instead
- [ ] Remove truly dead code
- [ ] Version control tracks code changes

### Dir 4.6 - Typedefs for Size/Signedness (Advisory)

**What it requires:** typedefs shall be used to indicate size and signedness.

**Why static analysis cannot fully verify:**
- Use of stdint.h types can be detected
- But verifying ALL numeric types are typedef'd requires review
- Project conventions may vary

**Manual review checklist:**
- [ ] Use int8_t, int16_t, int32_t, int64_t
- [ ] Use uint8_t, uint16_t, uint32_t, uint64_t
- [ ] Avoid bare int/long in portable code
- [ ] Document any exceptions

### Dir 4.7 - Error Information Testing (Required)

**What it requires:** Error information shall be tested.

**Partial static analysis support:**
- Can detect unused return values (automotive-missing-return-value-handling)
- Cannot verify error handling is complete/correct

**Manual review checklist:**
- [ ] All function return values checked
- [ ] Error codes propagated appropriately
- [ ] Recovery actions documented
- [ ] errno checked after library calls

### Dir 4.8 - Pointer Hiding (Advisory)

**What it requires:** Pointers should not be hidden in typedefs.

**Why static analysis has limitations:**
- Can detect typedef of pointer types
- May have false positives for opaque types
- Context-dependent

**Manual review checklist:**
- [ ] No `typedef int *IntPtr` patterns
- [ ] Opaque handles clearly documented
- [ ] Pointer nature visible at use site

### Dir 4.9 - Function-Like Macros (Advisory)

**What it requires:** Function-like macros should be avoided.

**Partial static analysis support:**
- Can detect function-like macros
- May have legitimate uses (performance, generics)

**Manual review checklist:**
- [ ] Prefer inline functions
- [ ] Document reasons for macro use
- [ ] Macros properly parenthesized
- [ ] No side effects in macro arguments

### Dir 4.10 - Header Guards (Required)

**What it requires:** Headers shall have include guards.

**Static analysis support:** Clang -Wheader-guard

**Manual review checklist:**
- [ ] All headers have #ifndef/#define/#endif
- [ ] Guard names are unique (based on file path)
- [ ] Alternative: #pragma once (if portable)

### Dir 4.11 - Argument Validity (Required)

**What it requires:** Validity of function arguments shall be checked.

**Why static analysis cannot fully verify:**
- Requires understanding of valid ranges
- Domain-specific constraints
- Runtime verification needed

**Manual review checklist:**
- [ ] Pointer parameters checked for NULL
- [ ] Array bounds parameters validated
- [ ] Enum parameters range-checked
- [ ] Numeric parameters validated against domain

### Dir 4.12 - Dynamic Memory (Required)

**What it requires:** Dynamic memory shall not be used.

**Static analysis support:** automotive-avoid-stdlib-malloc detects malloc/free/etc.

**Manual review checklist:**
- [ ] No malloc/calloc/realloc/free
- [ ] No strdup or similar
- [ ] Static allocation pools if needed
- [ ] Document any exceptions with justification

### Dir 4.13 - Atomic Operations (Advisory)

**What it requires:** Appropriate care shall be taken with atomic operations.

**Why static analysis cannot fully verify:**
- Correctness depends on design intent
- Memory ordering requirements context-dependent
- Race conditions hard to detect statically

**Manual review checklist:**
- [ ] Memory ordering appropriate for use case
- [ ] No torn reads/writes on shared data
- [ ] Lock-free algorithms verified
- [ ] Document concurrency assumptions

### Dir 4.14 - External Input Validation (Required)

**What it requires:** All external inputs shall be validated.

**Why static analysis cannot fully verify:**
- Cannot identify all external input sources
- Validation requirements are domain-specific
- Boundary between trusted/untrusted code varies

**Manual review checklist:**
- [ ] User input sanitized
- [ ] Network data validated
- [ ] File input checked
- [ ] Sensor data range-checked
- [ ] IPC message validated
- [ ] Configuration data verified

## Using This Document

### For Developers

1. Review this document when implementing features
2. Use the checklists during code review
3. Document compliance in code comments or separate documents

### For Reviewers

1. Verify checklist items are addressed
2. Request evidence of compliance
3. Document review findings

### For Auditors

1. Request compliance documentation
2. Sample code for manual review
3. Verify checklist completion

## Integration with clang-tidy-automotive

While these directives require manual review, clang-tidy-automotive provides partial support:

| Directive | Partial Automation |
|-----------|-------------------|
| Dir 4.7 | `automotive-missing-return-value-handling` |
| Dir 4.10 | Clang `-Wheader-guard` |
| Dir 4.12 | `automotive-avoid-stdlib-malloc` |

Configure these checks in `.clang-tidy`:

```yaml
Checks: >
  automotive-missing-return-value-handling,
  automotive-avoid-stdlib-malloc
```

## References

- MISRA C:2025 Guidelines
- ISO 26262 Software Development Process
- IEC 61508 Functional Safety

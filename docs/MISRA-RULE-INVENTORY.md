# MISRA C:2025 Rule Inventory

This document provides a comprehensive mapping between MISRA C:2025 rules and their implementation status in clang-tidy-automotive.

## Legend

| Status | Description |
|--------|-------------|
| **Implemented** | Custom check in clang-tidy-automotive |
| **Clang Built-in** | Covered by existing clang/clang-tidy checks |
| **Planned** | Scheduled for implementation |
| **Manual** | Requires manual code review |
| **N/A** | Not applicable to static analysis |

## Summary Statistics

| Category | Total | Implemented | Clang | Planned | Manual |
|----------|-------|-------------|-------|---------|--------|
| Directives | 18 | 2 | 0 | 3 | 13 |
| Mandatory | 4 | 2 | 2 | 0 | 0 |
| Required | 113 | 37 | 15 | 28 | 33 |
| Advisory | 41 | 17 | 8 | 9 | 7 |
| **Total** | **176** | **58** | **25** | **40** | **53** |

## Implemented Checks

### Statement Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-goto` | 15.1 | Advisory | Detects goto statements |
| `automotive-forward-goto-label` | 15.3 | Required | Detects forward jumps to labels |
| `automotive-missing-break-in-case` | 16.3 | Required | Detects fall-through in switch |
| `automotive-missing-default-in-switch` | 16.4 | Required | Detects switch without default |
| `automotive-c23-req-16.5` | 16.5 | Required | Wrong order in switch statement |
| `automotive-avoid-boolean-in-switch` | 16.7 | Required | Boolean expression in switch |
| `automotive-x-req-16.2` | 16.2 | Required | Unstructured switch statement |
| `automotive-missing-compound` | 15.6 | Required | Missing braces on control flow |
| `automotive-missing-else` | 15.7 | Required | Missing else after else-if |
| `automotive-unused-label` | 2.6 | Advisory | Unused labels |

### Expression Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-adv-13.4` | 13.4 | Advisory | Assignment used as expression |
| `automotive-c23-req-13.5` | 13.5 | Required | Side effects in logical operands |
| `automotive-c23-req-14.3` | 14.3 | Required | Invariant controlling expression |
| `automotive-c23-req-14.4` | 14.4 | Required | Non-boolean in condition |

### Operator Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-req-12.2` | 12.2 | Required | Shift operator bounds |
| `automotive-avoid-comma-operator` | 12.3 | Advisory | Comma operator usage |

### Literal Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-octal-number` | 7.1 | Required | Octal literal constants |
| `automotive-avoid-lowercase-literal-suffix` | 7.3 | Required | Lowercase 'l' suffix |
| `automotive-unterminated-escape-sequence` | 4.1 | Required | Incomplete escape sequences |

### Type Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-union` | 19.2 | Advisory | Union type usage |
| `automotive-implicit-int` | 8.1 | Required | Implicit int type |
| `automotive-unique-enum-value` | 8.12 | Required | Duplicate enum values |
| `automotive-unused-type` | 2.3 | Advisory | Unused type declarations |

### Bitfield Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-wrong-bitfield-type` | 6.1 | Required | Wrong bit-field type |
| `automotive-avoid-signed-single-bitfield` | 6.2 | Required | Single-bit signed bit-field |
| `automotive-avoid-bitfield-in-union` | 19.2 | Advisory | Bit-fields in unions |

### Pointer Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-req-11.8` | 11.8 | Required | Cast removing const/volatile |
| `automotive-wrong-null-pointer-value` | 11.9 | Required | Incorrect NULL pointer literal |

### Array Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-partial-array-init` | 9.3 | Required | Partial array initialization |
| `automotive-c23-adv-18.4` | 18.4 | Advisory | Pointer arithmetic |
| `automotive-avoid-flexible-array-member` | 18.7 | Required | Flexible array members |
| `automotive-avoid-variable-length-array` | 18.8 | Required | Variable-length arrays |
| `automotive-missing-external-array-size` | 8.11 | Advisory | External array without size |

### Function Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-uncomplete-function-prototype` | 8.2 | Required | Incomplete function prototype |
| `automotive-function-declaration-mismatch` | 8.3 | Required | Declaration/definition mismatch |
| `automotive-missing-static-inline` | 8.10 | Required | Static inline functions |
| `automotive-implicit-function-decl` | 17.3 | Mandatory | Implicit function declaration |
| `automotive-c23-mand-17.6` | 17.6 | Mandatory | Static in array parameters |
| `automotive-avoid-function-parameter-modification` | 17.8 | Advisory | Modifying function parameters |
| `automotive-avoid-multiple-return-stmt` | 15.5 | Advisory | Multiple return statements |
| `automotive-missing-return-value-handling` | 17.7 | Required | Ignoring return values |
| `automotive-missing-return-void` | 17.4 | Mandatory | Non-void function missing return |

### Storage Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-req-21.2` | 21.2 | Required | Reserved identifier declarations |
| `automotive-avoid-restrict-type` | 8.14 | Required | restrict qualifier usage |
| `automotive-missing-static-internal-linkage` | 8.8 | Required | Missing static for internal linkage |

### Preprocessor Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-code-before-include` | 20.1 | Advisory | Code before #include |
| `automotive-avoid-invalid-header-char` | 20.2 | Required | Invalid chars in header names |
| `automotive-avoid-macro-named-as-ckeyword` | 20.4 | Required | Macro named as C keyword |
| `automotive-avoid-undef` | 20.5 | Advisory | #undef usage |
| `automotive-avoid-hash-operator` | 20.10 | Advisory | # operator in macros |
| `automotive-avoid-multiple-hash-operators` | 20.11 | Required | Multiple # or ## operators |
| `automotive-avoid-reserved-macro-identifier` | 21.1 | Required | Reserved identifier in #define/#undef |
| `automotive-unused-macro` | 2.5 | Advisory | Unused macros |

### Comment Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-comment-within-comment` | 3.1 | Required | Comment markers in comments |
| `automotive-avoid-line-splicing-within-comment` | 3.2 | Required | Line splicing in comments |

### Standard Library Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-stdarg-header` | 17.1 | Required | <stdarg.h> usage |
| `automotive-avoid-setjmp-header` | 21.4 | Required | <setjmp.h> usage |
| `automotive-avoid-signal-header` | 21.5 | Required | <signal.h> usage |
| `automotive-avoid-ascii-to-number` | 21.7 | Required | atoi/atof functions |
| `automotive-avoid-stdlib-exit` | 21.8 | Required | abort/exit functions |
| `automotive-avoid-stdlib-system` | 21.8 | Required | system() function |
| `automotive-avoid-stdlib-rand` | 21.12 | Required | rand() function |

### Unused Code Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-x-adv-2.4` | 2.4 | Advisory | Unused tags |
| `automotive-c23-adv-2.7` | 2.7 | Advisory | Unused function parameters |
| `automotive-c23-adv-2.8` | 2.8 | Advisory | Unused object definitions |

### Reused Clang/LLVM Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-req-14.1` | 14.1 | Required | Float loop counter (cert-flp30-c) |
| `automotive-c23-req-17.2` | 17.2 | Required | No recursion (misc-no-recursion) |

## Rules Covered by Built-in Clang Warnings

These rules are enforced by standard clang warnings:

| MISRA Rule | Category | Clang Warning | Description |
|------------|----------|---------------|-------------|
| 1.1 | Required | -Werror | Standard C syntax violations |
| 1.3 | Required | -fsanitize=undefined | Undefined behavior |
| 2.1 | Required | -Wunreachable-code | Unreachable code |
| 5.1 | Required | -Widentifier-length | External identifier uniqueness |
| 8.5 | Required | Multiple definition errors | One definition rule |
| 10.3 | Required | -Wimplicit-conversion | Narrowing conversions |
| 11.1 | Required | -Wpointer-arith | Pointer/integer conversions |
| 12.1 | Advisory | -Wparentheses | Operator precedence |
| 17.5 | Advisory | -Warray-parameter | Array parameter mismatch |
| 22.1 | Required | Resource leak detection | Resource management |

## Rules Requiring Manual Review

These rules cannot be fully verified by static analysis:

| MISRA Rule | Category | Reason |
|------------|----------|--------|
| Dir 1.1 | Required | Implementation-defined behavior documentation |
| Dir 2.1 | Required | Assembly language isolation |
| Dir 3.1 | Required | Compilation with all warnings enabled |
| Dir 4.1 | Required | Run-time failure minimization |
| Dir 4.3 | Required | Assembly language encapsulation |
| Dir 4.4 | Advisory | Defensive programming |
| Dir 4.6 | Advisory | Typedefs for size/signedness |
| Dir 4.7 | Required | Error information testing |
| Dir 4.8 | Advisory | Pointer hiding |
| Dir 4.9 | Advisory | Function-like macros |
| Dir 4.10 | Required | Header guards |
| Dir 4.11 | Required | Validity of function arguments |
| Dir 4.12 | Required | Dynamic memory avoidance |
| Dir 4.13 | Advisory | Atomic operations |
| Dir 4.14 | Required | External input validation |
| 5.2-5.9 | Various | Identifier scope and visibility |
| 8.7 | Advisory | Internal linkage preference |
| 9.5 | Required | Designated initializer bounds |
| 22.2-22.10 | Various | Resource management patterns |

## Planned Implementations

### High Priority (Required Rules)
- Rule 10.1-10.8: Essential type model checks
- Rule 11.2-11.7: Pointer conversion checks (11.8 Implemented)
- ~~Rule 12.2: Shift operator checks~~ (Implemented)
- Rule 13.1-13.4, 13.6: Side effect checks (13.5 Implemented)
- Rule 18.1-18.3, 18.5-18.6: Pointer arithmetic checks (18.4 Implemented)
- ~~Rule 20.1-20.2: Preprocessor directive checks~~ (Implemented)
- Rule 20.3: Include syntax validation
- ~~Rule 21.1: Reserved macro identifiers~~ (Implemented)
- ~~Rule 21.2: Reserved identifier declarations~~ (Implemented)

### Medium Priority (Advisory Rules)
- Dir 4.2: Language subset selection
- ~~Rule 2.7: Unused parameters~~ (Implemented)
- Rule 4.2: Trigraph sequences
- Rule 8.9: Local object scope
- Rule 12.4: Constant expressions
- Rule 15.2: Goto in same block

## Integration with SonarQube

All implemented checks can be imported into SonarQube using:

1. **SARIF Format**: `scripts/clang-tidy-to-sarif.py`
2. **Generic Issue Format**: `scripts/clang-tidy-to-sonarqube.py`

See `docs/sonarqube-integration.md` for detailed setup instructions.

## Contributing

To add a new check:

1. Identify the MISRA rule number and category
2. Create the check class in the appropriate component directory
3. Register in the component's `*Component.cpp`
4. Add test file in `test/checkers/automotive/`
5. Add example files in `examples/rules/rule-XX/`
6. Update this inventory document

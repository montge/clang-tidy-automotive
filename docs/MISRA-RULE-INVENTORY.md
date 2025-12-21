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
| Directives | 18 | 5 | 0 | 0 | 13 |
| Mandatory | 4 | 5 | 0 | 0 | 0 |
| Required | 113 | 80 | 15 | 0 | 18 |
| Advisory | 41 | 27 | 8 | 0 | 6 |
| **Total** | **176** | **113** | **25** | **0** | **38** |

*Note: 113 MISRA C:2025 checks + 22 MISRA C++:2023 checks = 135 total automotive checks*

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
| `automotive-c23-adv-15.4` | 15.4 | Advisory | Multiple loop terminators |
| `automotive-c23-req-15.2` | 15.2 | Required | Goto and label in same block |
| `automotive-unused-label` | 2.6 | Advisory | Unused labels |
| `automotive-c23-req-16.6` | 16.6 | Required | Switch minimum clauses |

### Expression Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-req-10.1` | 10.1 | Required | Inappropriate essential type |
| `automotive-c23-req-10.2` | 10.2 | Required | Character arithmetic restrictions |
| `automotive-c23-req-10.3` | 10.3 | Required | Narrowing compound assignment |
| `automotive-c23-req-10.4` | 10.4 | Required | Essential type mismatch in operations |
| `automotive-c23-adv-10.5` | 10.5 | Advisory | Inappropriate cast between essential types |
| `automotive-c23-req-10.6` | 10.6-10.8 | Required | Composite expression type mismatch |
| `automotive-c23-adv-12.4` | 12.4 | Advisory | Constant expression wrap-around |
| `automotive-c23-req-13.1` | 13.1-13.3 | Required | Side effects in initializers |
| `automotive-c23-adv-13.4` | 13.4 | Advisory | Assignment used as expression |
| `automotive-c23-req-13.5` | 13.5 | Required | Side effects in logical operands |
| `automotive-c23-mand-13.6` | 13.6 | Mandatory | Side effects in sizeof operand |
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
| `automotive-c23-adv-4.2` | 4.2 | Advisory | Trigraph sequences |
| `automotive-avoid-octal-number` | 7.1 | Required | Octal literal constants |
| `automotive-c23-req-7.2` | 7.2 | Required | Unsigned literal without suffix |
| `automotive-avoid-lowercase-literal-suffix` | 7.3 | Required | Lowercase 'l' suffix |
| `automotive-c23-req-7.4` | 7.4 | Required | String literal to non-const pointer |
| `automotive-unterminated-escape-sequence` | 4.1 | Required | Incomplete escape sequences |

### Identifier Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-req-5.4` | 5.4 | Required | Macro identifier conflicts |
| `automotive-c23-req-5.6` | 5.6 | Required | Duplicate typedef names |
| `automotive-c23-req-5.7` | 5.7 | Required | Duplicate tag names |

### Type Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-union` | 19.2 | Advisory | Union type usage |
| `automotive-implicit-int` | 8.1 | Required | Implicit int type |
| `automotive-unique-enum-value` | 8.12 | Required | Duplicate enum values |
| `automotive-unused-type` | 2.3 | Advisory | Unused type declarations |
| `automotive-c23-req-9.2` | 9.2 | Required | Explicit enumerator values |
| `automotive-c23-req-11.10` | 11.10 | Required | Atomic void type restriction |

### Bitfield Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-wrong-bitfield-type` | 6.1 | Required | Wrong bit-field type |
| `automotive-avoid-signed-single-bitfield` | 6.2 | Required | Single-bit signed bit-field |
| `automotive-avoid-bitfield-in-union` | 19.2 | Advisory | Bit-fields in unions |

### Pointer Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-req-11.2` | 11.2, 11.5-11.7 | Required | Incompatible pointer conversions |
| `automotive-c23-req-11.3` | 11.3 | Required | Incompatible pointer cast |
| `automotive-c23-adv-11.4` | 11.4 | Advisory | Pointer to/from integer cast |
| `automotive-c23-req-11.8` | 11.8 | Required | Cast removing const/volatile |
| `automotive-wrong-null-pointer-value` | 11.9 | Required | Incorrect NULL pointer literal |
| `automotive-c23-req-11.11` | 11.11 | Required | Implicit NULL comparison |
| `automotive-c23-req-18.6` | 18.6 | Required | Address of automatic object escaping |

### Array Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-avoid-partial-array-init` | 9.3 | Required | Partial array initialization |
| `automotive-c23-req-9.4` | 9.4 | Required | Incomplete aggregate initialization |
| `automotive-c23-req-9.5` | 9.5 | Required | Designated initializer bounds |
| `automotive-c23-req-18.1` | 18.1-18.3, 18.5 | Required | Pointer arithmetic bounds |
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
| `automotive-c23-req-17.10` | 17.10 | Required | Noreturn function with non-void return |
| `automotive-c23-req-17.13` | 17.13 | Required | Qualified function type restriction |

### Storage Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-adv-8.7` | 8.7 | Advisory | Static for internal linkage |
| `automotive-c23-adv-8.9` | 8.9 | Advisory | Object at block scope |
| `automotive-c23-req-9.1` | 9.1 | Required | Uninitialized variable detection |
| `automotive-c23-req-21.2` | 21.2 | Required | Reserved identifier declarations |
| `automotive-avoid-restrict-type` | 8.14 | Required | restrict qualifier usage |

### Directive Checks
| Check ID | MISRA Directive | Category | Description |
|----------|-----------------|----------|-------------|
| `automotive-c23-req-dir-4.10` | Dir 4.10 | Required | Missing header guard protection |
| `automotive-c23-adv-dir-4.8` | Dir 4.8 | Advisory | Pointer hiding in typedefs |

### Preprocessor Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-c23-adv-1.2` | 1.2 | Advisory | Language extensions detection |
| `automotive-avoid-code-before-include` | 20.1 | Advisory | Code before #include |
| `automotive-avoid-invalid-header-char` | 20.2 | Required | Invalid chars in header names |
| `automotive-c23-req-20.3` | 20.3 | Required | Include syntax validation |
| `automotive-avoid-macro-named-as-ckeyword` | 20.4 | Required | Macro named as C keyword |
| `automotive-avoid-undef` | 20.5 | Advisory | #undef usage |
| `automotive-avoid-hash-operator` | 20.10 | Advisory | # operator in macros |
| `automotive-avoid-multiple-hash-operators` | 20.11 | Required | Multiple # or ## operators |
| `automotive-c23-req-20.14` | 20.14 | Required | #if/#endif in same file |
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
| `automotive-c23-req-21.3` | 21.3 | Required | Dynamic memory allocation |
| `automotive-avoid-setjmp-header` | 21.4 | Required | <setjmp.h> usage |
| `automotive-avoid-signal-header` | 21.5 | Required | <signal.h> usage |
| `automotive-c23-req-21.6` | 21.6 | Required | <stdio.h> input/output functions |
| `automotive-avoid-ascii-to-number` | 21.7 | Required | atoi/atof functions |
| `automotive-avoid-stdlib-exit` | 21.8 | Required | abort/exit functions |
| `automotive-avoid-stdlib-system` | 21.8 | Required | system() function |
| `automotive-c23-req-21.9` | 21.9 | Required | bsearch/qsort functions |
| `automotive-c23-req-21.10` | 21.10 | Required | <time.h> time functions |
| `automotive-c23-req-21.11` | 21.11 | Required | <tgmath.h> type-generic math |
| `automotive-avoid-stdlib-rand` | 21.12 | Required | rand() function |
| `automotive-c23-mand-21.13` | 21.13 | Mandatory | <ctype.h> character functions |
| `automotive-c23-req-21.21` | 21.21 | Required | <wchar.h> wide character functions |

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

### MISRA C++:2023 Checks
| Check ID | MISRA Rule | Category | Description |
|----------|------------|----------|-------------|
| `automotive-cpp23-req-9.3` | 9.3 | Required | Compound statement body required |
| `automotive-cpp23-req-9.4` | 9.4 | Required | Switch minimum clauses |
| `automotive-cpp23-req-12.3` | 12.3 | Required | Union prohibition |
| `automotive-cpp23-req-18.1` | 18.1 | Required | Exception prohibition |
| `automotive-cpp23-req-21.6` | 21.6 | Required | Dynamic memory prohibition |
| `automotive-cpp23-req-21.10` | 21.10 | Required | <ctime> prohibition |

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

## Missing Rules (Gap Analysis)

See `docs/MISRA-GAP-ANALYSIS.md` for detailed analysis.

### MISRA C:2025 Missing Rules Summary

**Missing Mandatory (9 rules):** 7.5, 9.7, 12.5, 17.9, 18.10, 19.1, 21.18, 21.22, 22.20

**Missing Required (38 rules):** 1.5, 2.2, 6.3, 7.6, 8.6, 8.18, 9.6, 12.6, 14.2, 16.1, 18.9, 19.3, 20.6, 20.7, 20.8, 20.9, 20.13, 20.15, 21.14, 21.23, 21.24, 21.25, 21.26, 22.11, 22.13, 22.15, 22.16, 22.17, 22.18, 22.19, 23.4, 23.6, 23.8

**Missing Advisory (10 rules):** 8.13, 8.16, 8.17, 8.19, 17.11, 17.12, 23.1, 23.3, 23.5, 23.7

**Missing Directives (2):** Dir 1.2, Dir 4.2

### MISRA C++:2023 Missing Rules Summary

**Missing Mandatory (1 rule):** 25.5

**Missing Required (34 rules):** 0.1, 0.3, 4.6, 5.7, 5.10, 5.13, 6.2, 6.4, 6.7, 7.0, 7.11, 8.7, 9.2, 9.5, 9.6, 10.1, 10.2, 10.4, 11.6, 12.2, 13.1, 13.3, 16.5, 17.8, 18.3, 19.1, 19.2, 19.3, 21.2, 22.3, 22.4, 28.3, 28.6, 30.0

**Missing Advisory (21 rules):** 0.0, 0.2, 4.1, 5.0, 5.7, 6.0, 6.5, 6.8, 6.9, 8.0, 8.1, 8.18, 8.19, 8.20, 10.3, 11.3, 14.1, 16.6, 18.5, 19.0, 26.3

**Missing Directives (2):** Dir 0.3, Dir 5.7

## Planned Implementations

### Phase 1: Quick Wins (Decidable, Low Complexity)

**MISRA C:2025 (9 implemented, 5 pending):**
- ~~Rule 7.2: Unsigned literal suffix~~ (Implemented)
- ~~Rule 7.4: String literal const assignment~~ (Implemented)
- ~~Rule 11.10: Atomic void restriction~~ (Implemented)
- ~~Rule 11.11: Implicit NULL comparison~~ (Implemented)
- ~~Rule 16.6: Switch minimum clauses~~ (Implemented)
- ~~Rule 17.10: Noreturn void return type~~ (Implemented)
- ~~Rule 17.13: Function type qualification~~ (Implemented)
- ~~Rule 20.14: Preprocessor directive file scope~~ (Implemented)
- ~~Rule 21.9: bsearch/qsort prohibition~~ (Implemented)
- Rule 6.3: Bit-field in union (already covered by `automotive-avoid-bitfield-in-union`)
- Rule 20.8: #if expression value (Deferred - complex expression analysis)
- Rule 20.9: #if undefined identifiers (Deferred - complex macro tracking)
- Rule 20.13: Valid preprocessing directive (Deferred - Clang already reports as errors)
- Rule 21.24: Random number prohibition (Pending)

**MISRA C++:2023 (6 implemented):**
- ~~Rule 9.3: Compound statement body~~ (Implemented)
- ~~Rule 9.4: Switch minimum clauses~~ (Implemented)
- ~~Rule 12.3: Union prohibition~~ (Implemented)
- ~~Rule 18.1: Exception prohibition~~ (Implemented)
- ~~Rule 21.6: Dynamic memory prohibition~~ (Implemented)
- ~~Rule 21.10: <ctime> prohibition~~ (Implemented)

### Phase 2: Medium Complexity

- Rule 1.5: Obsolescent features
- Rule 16.1: Well-formed switch
- Rule 14.2: Well-formed for loop
- Rule 20.7: Macro expansion parentheses
- Essential type checks for generics (23.x)

### Phase 3: Advanced Analysis

- Rule 2.2: Dead code detection
- Rule 9.7: Atomic initialization
- Rule 19.1/19.3: Overlapping storage
- Rule 22.x: Thread safety rules

### Completed Implementations

**Phase 1 Quick Wins (December 2025):**
- ~~Rule 7.2: Unsigned literal suffix~~ (Implemented)
- ~~Rule 7.4: String literal const assignment~~ (Implemented)
- ~~Rule 11.10: Atomic void restriction~~ (Implemented)
- ~~Rule 11.11: Implicit NULL comparison~~ (Implemented)
- ~~Rule 16.6: Switch minimum clauses~~ (Implemented)
- ~~Rule 17.10: Noreturn void return type~~ (Implemented)
- ~~Rule 17.13: Function type qualification~~ (Implemented)
- ~~Rule 20.14: Preprocessor directive file scope~~ (Implemented)
- ~~Rule 21.9: bsearch/qsort prohibition~~ (Implemented)
- ~~C++ Rule 9.3: Compound statement body~~ (Implemented)
- ~~C++ Rule 9.4: Switch minimum clauses~~ (Implemented)
- ~~C++ Rule 12.3: Union prohibition~~ (Implemented)
- ~~C++ Rule 18.1: Exception prohibition~~ (Implemented)
- ~~C++ Rule 21.6: Dynamic memory prohibition~~ (Implemented)
- ~~C++ Rule 21.10: <ctime> prohibition~~ (Implemented)

**Previous Implementations:**
- ~~Rule 10.2-10.8: Essential type model checks~~ (Implemented)
- ~~Rule 11.2-11.7: Pointer conversion checks~~ (Implemented)
- ~~Rule 12.2: Shift operator checks~~ (Implemented)
- ~~Rule 13.1-13.4, 13.6: Side effect checks~~ (Implemented)
- ~~Rule 18.1-18.3, 18.5-18.6: Pointer arithmetic checks~~ (Implemented)
- ~~Rule 20.1-20.3: Preprocessor directive checks~~ (Implemented)
- ~~Rule 21.1: Reserved macro identifiers~~ (Implemented)
- ~~Rule 21.2: Reserved identifier declarations~~ (Implemented)
- ~~Rule 18.6: Address of automatic object persisting past scope~~ (Implemented)
- ~~Rule 9.2: Explicit enumerator values~~ (Implemented)
- ~~Rule 9.4: Complete aggregate initialization~~ (Implemented)
- ~~Rule 9.5: Designated initializer bounds~~ (Implemented)
- ~~Rule 9.1: Uninitialized variable detection~~ (Implemented)
- ~~Rule 1.2: Language extensions~~ (Implemented)
- ~~Rule 2.7: Unused parameters~~ (Implemented)
- ~~Rule 4.2: Trigraph sequences~~ (Implemented)
- ~~Rule 8.7: Static for internal linkage~~ (Implemented)
- ~~Rule 8.9: Local object scope~~ (Implemented)
- ~~Rule 12.4: Constant expressions~~ (Implemented)
- ~~Rule 15.2: Goto in same block~~ (Implemented)
- ~~Dir 4.8: Pointer hiding in typedefs~~ (Implemented)
- ~~Dir 4.10: Header guards~~ (Implemented)

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

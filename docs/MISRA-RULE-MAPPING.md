# MISRA C:2025 Rule Mapping

This document maps clang-tidy-automotive checks to MISRA C:2025 rules and directives.

## Quick Reference

| Rule | Category | Check ID | Status |
|------|----------|----------|--------|
| 2.4 | Unused Code | `automotive-unused-type` | ✅ Implemented |
| 3.1 | Comments | `automotive-avoid-comment-within-comment` | ✅ Implemented |
| 3.2 | Comments | `automotive-avoid-line-splicing-within-comment` | ✅ Implemented |
| 4.1 | Characters | `automotive-unterminated-escape-sequence` | ✅ Implemented |
| 6.1 | Types | `automotive-wrong-bitfield-type` | ✅ Implemented |
| 6.2 | Types | `automotive-avoid-signed-single-bitfield` | ✅ Implemented |
| 7.1 | Literals | `automotive-avoid-octal-number` | ✅ Implemented |
| 7.3 | Literals | `automotive-avoid-lowercase-literal-suffix` | ✅ Implemented |
| 8.1 | Declarations | `automotive-implicit-function-decl`, `automotive-implicit-int` | ✅ Implemented |
| 8.2 | Declarations | `automotive-uncomplete-function-prototype` | ✅ Implemented |
| 8.3 | Declarations | `automotive-function-declaration-mismatch` | ✅ Implemented |
| 8.7 | Declarations | `automotive-missing-static-internal-linkage` | ✅ Implemented |
| 8.9 | Declarations | `automotive-missing-static-inline` | ✅ Implemented |
| 8.14 | Declarations | `automotive-avoid-restrict-type` | ✅ Implemented |
| 9.5 | Initialization | `automotive-unique-enum-value` | ✅ Implemented |
| 11.9 | Pointers | `automotive-wrong-null-pointer-value` | ✅ Implemented |
| 12.3 | Expressions | `automotive-avoid-comma-operator` | ✅ Implemented |
| 13.4 | Side Effects | `automotive-c23-adv-13.4` | ✅ Implemented |
| 14.1 | Control Flow | `automotive-c23-req-14.1` (alias: cert-flp30-c) | ✅ Implemented |
| 14.2 | Control Flow | `automotive-c23-req-14.3` | ✅ Implemented |
| 14.4 | Control Flow | `automotive-c23-req-14.4` | ✅ Implemented |
| 15.1 | Statements | `automotive-avoid-goto` | ✅ Implemented |
| 15.5 | Statements | `automotive-avoid-multiple-return-stmt` | ✅ Implemented |
| 15.6 | Statements | `automotive-missing-compound` | ✅ Implemented |
| 16.2 | Switch | `automotive-x-req-16.2` | ✅ Implemented |
| 16.4 | Switch | `automotive-missing-default-in-switch` | ✅ Implemented |
| 16.5 | Switch | `automotive-c23-req-16.5` | ✅ Implemented |
| 17.1 | Functions | `automotive-avoid-stdarg-header` | ✅ Implemented |
| 17.2 | Functions | `automotive-c23-req-17.2` (alias: misc-no-recursion) | ✅ Implemented |
| 17.7 | Functions | `automotive-missing-return-value-handling` | ✅ Implemented |
| 17.8 | Functions | `automotive-avoid-function-parameter-modification` | ✅ Implemented |
| 18.7 | Pointers/Arrays | `automotive-avoid-atomic-void-pointer` | ✅ Implemented |
| 18.8 | Pointers/Arrays | `automotive-avoid-variable-length-array` | ✅ Implemented |
| 19.2 | Overlapping | `automotive-avoid-union`, `automotive-avoid-bitfield-in-union` | ✅ Implemented |
| 20.4 | Preprocessor | `automotive-avoid-macro-named-as-ckeyword` | ✅ Implemented |
| 20.5 | Preprocessor | `automotive-avoid-undef` | ✅ Implemented |
| 20.10 | Preprocessor | `automotive-avoid-hash-operator` | ✅ Implemented |
| 20.11 | Preprocessor | `automotive-avoid-multiple-hash-operators` | ✅ Implemented |
| 21.3 | Std Library | `automotive-avoid-stdlib-malloc` | ✅ Implemented |
| 21.4 | Std Library | `automotive-avoid-setjmp-header` | ✅ Implemented |
| 21.5 | Std Library | `automotive-avoid-signal-header` | ✅ Implemented |
| 21.7 | Std Library | `automotive-avoid-ascii-to-number` | ✅ Implemented |
| 21.8 | Std Library | `automotive-avoid-stdlib-exit`, `automotive-avoid-stdlib-system` | ✅ Implemented |
| 21.24 | Std Library | `automotive-avoid-stdlib-rand` | ✅ Implemented |

## Detailed Mapping by Category

### Environment (Rules 1.x-2.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 2.4 | Unused type declarations | `automotive-unused-type` | Advisory |
| 2.8 | Unused object definitions | `automotive-c23-adv-2.8` | Advisory |

### Comments (Rules 3.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 3.1 | Comment sequences in comments | `automotive-avoid-comment-within-comment` | Required |
| 3.2 | Line-splicing in // comments | `automotive-avoid-line-splicing-within-comment` | Required |

### Characters (Rules 4.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 4.1 | Escape sequence validity | `automotive-unterminated-escape-sequence` | Required |

### Types (Rules 6.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 6.1 | Bit-field types | `automotive-wrong-bitfield-type` | Required |
| 6.2 | Signed single-bit fields | `automotive-avoid-signed-single-bitfield` | Required |

### Literals (Rules 7.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 7.1 | Octal constants | `automotive-avoid-octal-number` | Required |
| 7.3 | Lowercase 'l' suffix | `automotive-avoid-lowercase-literal-suffix` | Required |

### Declarations (Rules 8.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 8.1 | Explicit types | `automotive-implicit-int` | Required |
| 8.1 | Function prototypes | `automotive-implicit-function-decl` | Required |
| 8.2 | Prototype form | `automotive-uncomplete-function-prototype` | Required |
| 8.3 | Compatible types | `automotive-function-declaration-mismatch` | Required |
| 8.7 | External linkage | `automotive-missing-static-internal-linkage` | Advisory |
| 8.9 | Static inline | `automotive-missing-static-inline` | Advisory |
| 8.14 | Restrict qualifier | `automotive-avoid-restrict-type` | Required |

### Initialization (Rules 9.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 9.5 | Unique enum values | `automotive-unique-enum-value` | Required |

### Pointers (Rules 11.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 11.9 | NULL vs 0 | `automotive-wrong-null-pointer-value` | Advisory |

### Expressions (Rules 12.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 12.3 | Comma operator | `automotive-avoid-comma-operator` | Advisory |

### Side Effects (Rules 13.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 13.4 | Assignment result | `automotive-c23-adv-13.4` | Advisory |

### Control Flow (Rules 14.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 14.1 | Float loop counter | `automotive-c23-req-14.1` | Required |
| 14.2 | Well-formed for loops | `automotive-c23-req-14.3` | Required |
| 14.4 | Boolean conditions | `automotive-c23-req-14.4` | Required |

### Statements (Rules 15.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 15.1 | Goto statement | `automotive-avoid-goto` | Advisory |
| 15.5 | Single exit point | `automotive-avoid-multiple-return-stmt` | Advisory |
| 15.6 | Compound statements | `automotive-missing-compound` | Required |

### Switch (Rules 16.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 16.2 | Switch structure | `automotive-x-req-16.2` | Required |
| 16.4 | Default case | `automotive-missing-default-in-switch` | Required |
| 16.5 | Switch ordering | `automotive-c23-req-16.5` | Required |

### Functions (Rules 17.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 17.1 | stdarg.h | `automotive-avoid-stdarg-header` | Required |
| 17.2 | Recursion | `automotive-c23-req-17.2` | Required |
| 17.7 | Return value usage | `automotive-missing-return-value-handling` | Required |
| 17.8 | Parameter modification | `automotive-avoid-function-parameter-modification` | Advisory |

### Pointers and Arrays (Rules 18.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 18.7 | Atomic void pointers | `automotive-avoid-atomic-void-pointer` | Required |
| 18.8 | VLAs | `automotive-avoid-variable-length-array` | Required |

### Overlapping Storage (Rules 19.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 19.2 | Unions | `automotive-avoid-union` | Advisory |
| 19.2 | Bit-fields in unions | `automotive-avoid-bitfield-in-union` | Required |

### Preprocessor (Rules 20.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 20.4 | Keyword macros | `automotive-avoid-macro-named-as-ckeyword` | Required |
| 20.5 | #undef | `automotive-avoid-undef` | Advisory |
| 20.10 | # operator | `automotive-avoid-hash-operator` | Advisory |
| 20.11 | ## operator | `automotive-avoid-multiple-hash-operators` | Advisory |

### Standard Library (Rules 21.x)

| Rule | Description | Check | Notes |
|------|-------------|-------|-------|
| 21.3 | Dynamic memory | `automotive-avoid-stdlib-malloc` | Required |
| 21.4 | setjmp.h | `automotive-avoid-setjmp-header` | Required |
| 21.5 | signal.h | `automotive-avoid-signal-header` | Required |
| 21.7 | atoi/atof/atol | `automotive-avoid-ascii-to-number` | Required |
| 21.8 | exit/abort | `automotive-avoid-stdlib-exit` | Required |
| 21.8 | system() | `automotive-avoid-stdlib-system` | Required |
| 21.24 | rand/srand | `automotive-avoid-stdlib-rand` | Required |

## Coverage Summary

| Category | Total Rules | Implemented | Coverage |
|----------|-------------|-------------|----------|
| Environment (1-2) | 8 | 2 | 25% |
| Comments (3) | 2 | 2 | 100% |
| Characters (4) | 2 | 1 | 50% |
| Identifiers (5) | 9 | 0 | 0% |
| Types (6) | 2 | 2 | 100% |
| Literals (7) | 4 | 2 | 50% |
| Declarations (8) | 14 | 7 | 50% |
| Initialization (9) | 5 | 1 | 20% |
| Conversions (10) | 8 | 0 | 0% |
| Pointers (11) | 9 | 1 | 11% |
| Expressions (12) | 4 | 1 | 25% |
| Side Effects (13) | 6 | 1 | 17% |
| Control Flow (14) | 4 | 3 | 75% |
| Statements (15) | 7 | 3 | 43% |
| Switch (16) | 7 | 3 | 43% |
| Functions (17) | 8 | 4 | 50% |
| Pointers/Arrays (18) | 8 | 2 | 25% |
| Overlapping (19) | 2 | 2 | 100% |
| Preprocessor (20) | 14 | 4 | 29% |
| Standard Library (21) | 24 | 7 | 29% |
| Resources (22) | 10 | 0 | 0% |
| Atomics (23) | 4 | 0 | 0% |

**Overall: ~46 rules implemented out of ~181 total (~25%)**

## Not Yet Implemented

### High Priority (Required Rules)

- Rules 5.x (Identifiers) - 9 rules
- Rules 10.x (Conversions) - 8 rules
- Rules 22.x (Resources) - 10 rules
- Rules 23.x (Atomics) - 4 rules

### Medium Priority

- Remaining Rules 8.x
- Remaining Rules 11.x
- Remaining Rules 18.x
- Remaining Rules 20.x

## Doxygen Integration

Each check header includes:
- `@brief` - Short description
- `Related MISRA C:2025 Rule:` - Rule number and description
- `@code` / `@endcode` - Example violation

To find all checks for a specific rule category, use:
```bash
grep -r "Related MISRA C:2025 Rule: 21" src/automotive/
```

## Search by Rule Number

```bash
# Find check for rule 15.1
grep -r "Rule: 15.1" src/automotive/

# Find all standard library rules
grep -r "Rule: 21\." src/automotive/
```

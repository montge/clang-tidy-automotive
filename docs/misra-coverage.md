---
layout: default
title: MISRA Coverage
nav_order: 2
---

# MISRA C:2025 Coverage
{: .no_toc }

Track implementation status of MISRA C:2025 rules.
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Coverage Overview

| Status | Count | Percentage |
|:-------|:------|:-----------|
| ✅ Implemented | 46 | ~25% |
| 🔶 Partial | 5 | ~3% |
| ⬜ Not Implemented | 130 | ~72% |

---

## Rules by Category

### Environment & Unused Code (Rules 1-2)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 2.4 | Unused type declarations | ✅ | `automotive-unused-type` |
| 2.8 | Unused object definitions | ✅ | `automotive-c23-adv-2.8` |

### Comments (Rules 3)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 3.1 | Comment sequences in comments | ✅ | `automotive-avoid-comment-within-comment` |
| 3.2 | Line-splicing in // comments | ✅ | `automotive-avoid-line-splicing-within-comment` |

{: .note }
Comments category has 100% coverage.

### Characters (Rules 4)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 4.1 | Escape sequence validity | ✅ | `automotive-unterminated-escape-sequence` |
| 4.2 | Trigraph sequences | ⬜ | - |

### Identifiers (Rules 5)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 5.1 | External identifier uniqueness | ⬜ | - |
| 5.2 | Internal identifier uniqueness | ⬜ | - |
| 5.3 | Identifier hiding | ⬜ | - |

{: .warning }
Identifier rules require whole-program analysis and are complex to implement.

### Types (Rules 6)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 6.1 | Bit-field types | ✅ | `automotive-wrong-bitfield-type` |
| 6.2 | Signed single-bit fields | ✅ | `automotive-avoid-signed-single-bitfield` |

{: .note }
Types category has 100% coverage.

### Literals (Rules 7)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 7.1 | Octal constants | ✅ | `automotive-avoid-octal-number` |
| 7.2 | Unsigned suffix | ⬜ | - |
| 7.3 | Lowercase 'l' suffix | ✅ | `automotive-avoid-lowercase-literal-suffix` |
| 7.4 | String literal assignment | ⬜ | - |

### Declarations (Rules 8)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 8.1 | Explicit types | ✅ | `automotive-implict-int` |
| 8.1 | Function prototypes | ✅ | `automotive-implicit-function-decl` |
| 8.2 | Prototype form | ✅ | `automotive-uncomplete-function-prototype` |
| 8.3 | Compatible types | ✅ | `automotive-function-declaration-mismatch` |
| 8.7 | External linkage | ✅ | `automotive-missing-static-internal-linkage` |
| 8.9 | Static inline | ✅ | `automotive-static-inline` |
| 8.14 | Restrict qualifier | ✅ | `automotive-avoid-restrict-type` |

### Statements (Rules 15)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 15.1 | Goto statement | ✅ | `automotive-avoid-goto` |
| 15.5 | Single exit point | ✅ | `automotive-avoid-multiple-return-stmt` |
| 15.6 | Compound statements | ✅ | `automotive-missing-compound` |

### Switch (Rules 16)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 16.2 | Switch structure | ✅ | `automotive-x-req-16.2` |
| 16.4 | Default case | ✅ | `automotive-missing-default-in-switch` |
| 16.5 | Switch ordering | ✅ | `automotive-c23-req-16.5` |

### Standard Library (Rules 21)

| Rule | Description | Status | Check ID |
|:-----|:------------|:-------|:---------|
| 21.3 | Dynamic memory | ✅ | `automotive-avoid-stdlib-malloc-call` |
| 21.4 | setjmp.h | ✅ | `automotive-avoid-setjmp-header` |
| 21.5 | signal.h | ✅ | `automotive-avoid-signal-header` |
| 21.7 | atoi/atof/atol | ✅ | `automotive-avoid-ascii-to-number` |
| 21.8 | exit/abort | ✅ | `automotive-avoid-stdlib-exit-call` |
| 21.8 | system() | ✅ | `automotive-avoid-stdlib-system-call` |
| 21.24 | rand/srand | ✅ | `automotive-avoid-stdlib-rand-call` |

---

## Not Yet Implemented

### High Priority

These Required rules are not yet implemented:

- **Rules 5.x** (Identifiers) - 9 rules
- **Rules 10.x** (Conversions) - 8 rules
- **Rules 22.x** (Resources) - 10 rules
- **Rules 23.x** (Atomics) - 4 rules

### Medium Priority

- Remaining Rules 8.x
- Remaining Rules 11.x
- Remaining Rules 18.x
- Remaining Rules 20.x

---

## Traceability

Each check links to its MISRA rule:

```
MISRA Rule 15.1 → automotive-avoid-goto → test/statement/avoid-goto.c
```

See [Traceability Matrix]({% link traceability.md %}) for full mapping.

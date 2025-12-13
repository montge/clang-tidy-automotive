---
layout: default
title: Checks
nav_order: 3
has_children: true
---

# Check Reference
{: .no_toc }

All implemented clang-tidy automotive checks.
{: .fs-6 .fw-300 }

---

## Check Categories

| Category | Checks | Description |
|:---------|:-------|:------------|
| [Statements]({% link checks/statements.md %}) | 10 | Control flow, goto, switch |
| [Standard Library]({% link checks/stdlib.md %}) | 8 | malloc, signal, setjmp |
| [Preprocessor]({% link checks/preprocessor.md %}) | 6 | Macros, #undef, operators |
| [Functions]({% link checks/functions.md %}) | 5 | Prototypes, parameters |
| [Types]({% link checks/types.md %}) | 4 | Bit-fields, unions |
| [Declarations]({% link checks/declarations.md %}) | 4 | Storage, linkage |
| [Literals]({% link checks/literals.md %}) | 3 | Octal, suffixes |
| [Expressions]({% link checks/expressions.md %}) | 3 | Operators, conditions |

---

## Quick Reference

### All Check IDs

```
automotive-avoid-ascii-to-number
automotive-avoid-bitfield-in-union
automotive-avoid-boolean-in-switch
automotive-avoid-comma-operator
automotive-avoid-comment-within-comment
automotive-avoid-flexible-array-member
automotive-avoid-function-parameter-modification
automotive-avoid-goto
automotive-avoid-hash-operator
automotive-avoid-line-splicing-within-comment
automotive-avoid-lowercase-literal-suffix
automotive-avoid-macro-named-as-ckeyword
automotive-avoid-multiple-hash-operators
automotive-avoid-multiple-return-stmt
automotive-avoid-octal-number
automotive-avoid-partial-array-init
automotive-avoid-restrict-type
automotive-avoid-setjmp-header
automotive-avoid-signal-header
automotive-avoid-signed-single-bitfield
automotive-avoid-stdarg-header
automotive-avoid-stdlib-exit-call
automotive-avoid-stdlib-malloc-call
automotive-avoid-stdlib-rand-call
automotive-avoid-stdlib-system-call
automotive-avoid-undef
automotive-avoid-union
automotive-avoid-variable-length-array
automotive-forward-goto-label
automotive-function-declaration-mismatch
automotive-implicit-function-decl
automotive-implict-int
automotive-missing-break-in-case
automotive-missing-compound
automotive-missing-default-in-switch
automotive-missing-else
automotive-missing-external-array-size
automotive-missing-return-value-handling
automotive-missing-static-internal-linkage
automotive-no-return-void
automotive-static-inline
automotive-uncomplete-function-prototype
automotive-unique-enum-value
automotive-unterminated-escape-sequence
automotive-unused-label
automotive-unused-macro
automotive-unused-type
automotive-wrong-bitfield-type
automotive-wrong-null-pointer-value
```

---

## Using Checks

### Enable All

```bash
clang-tidy -checks="automotive-*" file.c --
```

### Enable Category

```bash
# All "avoid" checks
clang-tidy -checks="automotive-avoid-*" file.c --

# All "missing" checks
clang-tidy -checks="automotive-missing-*" file.c --
```

### Disable Specific

```bash
clang-tidy -checks="automotive-*,-automotive-avoid-goto" file.c --
```

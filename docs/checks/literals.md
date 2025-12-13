---
layout: default
title: Literal Checks
parent: Checks
nav_order: 7
---

# Literal Checks
{: .no_toc }

Checks for literal rules (MISRA C:2025 Rules 7.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-avoid-octal-number

Detects octal literal constants.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 7.1 |
| **Category** | Required |

### Rationale

Octal constants (leading zero) are confusing:
- `010` is 8, not 10
- Easy to accidentally create
- Often not intended

### Example

{: .warning }
> **Violation**

```c
int x = 0123;  // Warning: octal (decimal 83)
int y = 0777;  // Warning: octal (decimal 511)
```

{: .note }
> **Compliant**

```c
int x = 123;     // Decimal
int y = 0x1FF;   // Hexadecimal
int z = 0;       // Zero is OK
```

---

## automotive-avoid-lowercase-literal-suffix

Detects lowercase literal suffixes.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 7.3 |
| **Category** | Required |

### Rationale

Lowercase `l` looks like digit `1`:
- `100l` vs `1001`
- Easy to misread

### Example

{: .warning }
> **Violation**

```c
long a = 100l;    // Warning: lowercase l
float b = 1.0f;   // Warning: lowercase f
```

{: .note }
> **Compliant**

```c
long a = 100L;    // OK: uppercase L
float b = 1.0F;   // OK: uppercase F
```

---

## automotive-unterminated-escape-sequence

Detects invalid escape sequences.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 4.1 |
| **Category** | Required |

### Example

{: .warning }
> **Violation**

```c
char *s = "test\q";  // Warning: \q is invalid
```

### Valid Escape Sequences

| Sequence | Meaning |
|:---------|:--------|
| `\n` | Newline |
| `\t` | Tab |
| `\\` | Backslash |
| `\"` | Quote |
| `\0` | Null |
| `\xNN` | Hex |
| `\NNN` | Octal |

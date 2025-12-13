---
layout: default
title: Type Checks
parent: Checks
nav_order: 3
---

# Type Checks
{: .no_toc }

Checks for type-related rules (MISRA C:2025 Rules 6.x, 19.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-wrong-bitfield-type

Detects bit-fields declared with inappropriate types.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 6.1 |
| **Category** | Required |

### Appropriate Types

Only these types should be used for bit-fields:
- `unsigned int`
- `signed int`
- `_Bool`

---

## automotive-avoid-signed-single-bitfield

Detects single-bit bit-fields with signed types.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 6.2 |
| **Category** | Required |

### Rationale

A signed single-bit field can only hold 0 or -1, which is rarely intended.

### Example

{: .warning }
> **Violation**

```c
struct Flags {
    int flag : 1;  // Warning: can be 0 or -1
};
```

{: .note }
> **Compliant**

```c
struct Flags {
    unsigned int flag : 1;  // OK: can be 0 or 1
    _Bool enable : 1;       // OK: boolean
};
```

---

## automotive-avoid-union

Detects usage of union types.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 19.2 |
| **Category** | Advisory |

### Rationale

Unions allow type punning which:
- Has implementation-defined behavior
- Can violate strict aliasing
- Makes code hard to analyze

### Example

{: .warning }
> **Violation**

```c
union Data {  // Warning: union usage
    int i;
    float f;
};
```

---

## automotive-avoid-bitfield-in-union

Detects bit-field members within unions.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 19.2 |
| **Category** | Required |

### Example

{: .warning }
> **Violation**

```c
union Mixed {
    unsigned int flags : 8;  // Warning: bit-field in union
    int value;
};
```

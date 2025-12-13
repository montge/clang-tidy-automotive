---
layout: default
title: Function Checks
parent: Checks
nav_order: 5
---

# Function Checks
{: .no_toc }

Checks for function rules (MISRA C:2025 Rules 8.x, 17.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-implicit-function-decl

Detects calls to functions without prior declarations.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 8.1 |
| **Category** | Required |
| **Language** | C90 only |

### Rationale

Implicit function declarations assume `int` return type, which can cause:
- Type mismatches
- Undefined behavior
- Hard-to-find bugs

---

## automotive-uncomplete-function-prototype

Detects function prototypes without complete parameter information.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 8.2 |
| **Category** | Required |

### Example

{: .warning }
> **Violation**

```c
int func();      // Warning: empty parameter list
int foo(a, b);   // Warning: K&R style
```

{: .note }
> **Compliant**

```c
int func(void);        // OK: explicit void
int foo(int a, int b); // OK: named parameters
```

---

## automotive-function-declaration-mismatch

Detects mismatches between function declarations and definitions.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 8.3 |
| **Category** | Required |

### Example

{: .warning }
> **Violation**

```c
void func(int x);      // Declaration
void func(long x) {}   // Warning: type mismatch
```

---

## automotive-missing-static-inline

Detects inline functions without static specifier.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 8.9 |
| **Category** | Advisory |

### Rationale

Inline functions without `static` may cause:
- Multiple definition errors
- ODR violations

### Example

{: .warning }
> **Violation**

```c
inline int func(void) { return 0; }  // Warning
```

{: .note }
> **Compliant**

```c
static inline int func(void) { return 0; }  // OK
```

---

## automotive-avoid-function-parameter-modification

Detects modifications to function parameters.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 17.8 |
| **Category** | Advisory |

### Rationale

Modifying parameters obscures original values:
- Harder to debug
- Confusing code

### Example

{: .warning }
> **Violation**

```c
void func(int x) {
    x = x + 1;  // Warning: modifying parameter
}
```

{: .note }
> **Compliant**

```c
void func(int x) {
    int local = x + 1;  // OK: use local copy
}
```

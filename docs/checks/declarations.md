---
layout: default
title: Declaration Checks
parent: Checks
nav_order: 6
---

# Declaration Checks
{: .no_toc }

Checks for declaration rules (MISRA C:2025 Rules 8.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-implicit-int

Detects declarations with implicit int type.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 8.1 |
| **Category** | Required |

### Example

{: .warning }
> **Violation**

```c
const x = 5;     // Warning: implicit int
static y = 10;   // Warning: implicit int
```

{: .note }
> **Compliant**

```c
const int x = 5;
static int y = 10;
```

---

## automotive-missing-static-internal-linkage

Detects objects/functions that could have internal linkage.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 8.7 |
| **Category** | Advisory |

### Rationale

Functions and objects used only in one translation unit should be `static`:
- Prevents name collisions
- Enables better optimization
- Reduces symbol table size

### Example

{: .warning }
> **Violation**

```c
// helper.c - only used internally
int helper_func(void) { return 0; }  // Warning: could be static
```

{: .note }
> **Compliant**

```c
static int helper_func(void) { return 0; }  // OK
```

---

## automotive-avoid-restrict-type

Detects usage of the restrict qualifier.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 8.14 |
| **Category** | Required |

### Rationale

The `restrict` qualifier is error-prone:
- Incorrect usage causes undefined behavior
- Compiler can't verify the contract
- Hard to maintain

### Example

{: .warning }
> **Violation**

```c
void func(int *restrict ptr) {}  // Warning: restrict usage
```

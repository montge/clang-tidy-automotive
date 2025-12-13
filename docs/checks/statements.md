---
layout: default
title: Statement Checks
parent: Checks
nav_order: 1
---

# Statement Checks
{: .no_toc }

Checks for control statements (MISRA C:2025 Rules 15.x, 16.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-avoid-goto

Detects usage of goto statements.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 15.1 |
| **Category** | Advisory |
| **Decidable** | Yes |

### Example

{: .warning }
> **Violation**

```c
void func(void) {
    goto error;  // Warning: goto statement detected
error:
    return;
}
```

{: .note }
> **Compliant**

```c
void func(void) {
    if (has_error()) {
        handle_error();
        return;
    }
}
```

---

## automotive-avoid-multiple-return-stmt

Detects functions with multiple return statements.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 15.5 |
| **Category** | Advisory |
| **Decidable** | Yes |

### Example

{: .warning }
> **Violation**

```c
int func(int x) {
    if (x < 0) {
        return -1;  // Warning: multiple returns
    }
    return x;
}
```

{: .note }
> **Compliant**

```c
int func(int x) {
    int result;
    if (x < 0) {
        result = -1;
    } else {
        result = x;
    }
    return result;
}
```

---

## automotive-missing-default-in-switch

Detects switch statements without a default case.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 16.4 |
| **Category** | Required |
| **Decidable** | Yes |

### Example

{: .warning }
> **Violation**

```c
void func(int x) {
    switch (x) {  // Warning: no default case
        case 1: break;
        case 2: break;
    }
}
```

{: .note }
> **Compliant**

```c
void func(int x) {
    switch (x) {
        case 1: break;
        case 2: break;
        default: break;
    }
}
```

---

## automotive-missing-compound

Detects control statements without braces.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 15.6 |
| **Category** | Required |
| **Decidable** | Yes |

### Example

{: .warning }
> **Violation**

```c
void func(int x) {
    if (x > 0)
        x++;  // Warning: no braces
}
```

{: .note }
> **Compliant**

```c
void func(int x) {
    if (x > 0) {
        x++;
    }
}
```

---

## automotive-missing-break-in-case

Detects switch cases without break/return.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 16.3 |
| **Category** | Required |
| **Decidable** | Yes |

### Example

{: .warning }
> **Violation**

```c
void func(int x) {
    switch (x) {
        case 1:
            do_something();
            // Warning: fallthrough without break
        case 2:
            do_other();
            break;
    }
}
```

---

## automotive-unused-label

Detects labels that are never used.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 2.6 |
| **Category** | Advisory |
| **Decidable** | Yes |

### Example

{: .warning }
> **Violation**

```c
void func(void) {
unused_label:  // Warning: label never used
    return;
}
```

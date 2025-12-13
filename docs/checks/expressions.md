---
layout: default
title: Expression Checks
parent: Checks
nav_order: 8
---

# Expression Checks
{: .no_toc }

Checks for expression rules (MISRA C:2025 Rules 12.x, 13.x, 14.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-avoid-comma-operator

Detects usage of the comma operator.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 12.3 |
| **Category** | Advisory |

### Rationale

The comma operator:
- Obscures control flow
- Makes code harder to read
- Often indicates code smell

### Example

{: .warning }
> **Violation**

```c
int x = (a = 1, b = 2, a + b);  // Warning
```

{: .note }
> **Compliant**

```c
a = 1;
b = 2;
int x = a + b;
```

---

## automotive-c23-adv-13.4

Detects assignment used as expression value.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 13.4 |
| **Category** | Advisory |
| **Check ID** | `automotive-c23-adv-13.4` |

### Rationale

Assignment in expressions:
- Easy to confuse with `==`
- Reduces readability
- Common source of bugs

### Example

{: .warning }
> **Violation**

```c
if (x = get_value()) {}  // Warning: assignment in condition
int y = (z = 10) + 5;    // Warning: assignment in expression
```

{: .note }
> **Compliant**

```c
x = get_value();
if (x != 0) {}

z = 10;
int y = z + 5;
```

---

## automotive-c23-req-14.4

Detects non-boolean expressions in conditions.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 14.4 |
| **Category** | Required |
| **Check ID** | `automotive-c23-req-14.4` |

### Rationale

Conditions should be explicitly boolean:
- Improves clarity
- Catches logic errors
- Self-documenting code

### Example

{: .warning }
> **Violation**

```c
int x;
if (x) {}      // Warning: integer in condition
if (ptr) {}    // Warning: pointer in condition
```

{: .note }
> **Compliant**

```c
int x;
if (x != 0) {}     // OK: explicit comparison
if (ptr != NULL) {} // OK: explicit comparison
```

---

## automotive-c23-req-14.3

Detects invariant control expressions.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 14.2 |
| **Category** | Required |
| **Check ID** | `automotive-c23-req-14.3` |

### Rationale

Constant loop conditions usually indicate:
- Forgotten variable update
- Intentional infinite loop (should use `for(;;)`)

### Example

{: .warning }
> **Violation**

```c
while (1) {}    // Warning: constant condition
if (2 > 1) {}   // Warning: always true
```

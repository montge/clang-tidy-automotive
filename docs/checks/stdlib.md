---
layout: default
title: Standard Library Checks
parent: Checks
nav_order: 2
---

# Standard Library Checks
{: .no_toc }

Checks for standard library usage (MISRA C:2025 Rules 21.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-avoid-stdlib-malloc

Detects usage of dynamic memory allocation functions.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 21.3 |
| **Category** | Required |
| **Functions** | malloc, calloc, realloc, aligned_alloc, free |

### Rationale

Dynamic memory allocation is unpredictable in embedded systems:
- Allocation can fail at runtime
- Memory fragmentation over time
- Difficult to analyze memory usage statically

### Example

{: .warning }
> **Violation**

```c
void func(void) {
    int *p = malloc(100);  // Warning
    p = realloc(p, 200);   // Warning
    free(p);               // Warning
}
```

{: .note }
> **Compliant**

```c
static int buffer[100];  // Static allocation

void func(void) {
    // Use static buffer instead
}
```

---

## automotive-avoid-setjmp-header

Detects inclusion of `<setjmp.h>` and usage of setjmp/longjmp.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 21.4 |
| **Category** | Required |
| **Functions** | setjmp, longjmp |

### Rationale

setjmp/longjmp bypass normal control flow:
- Can cause resource leaks
- Difficult to reason about program state
- Stack unwinding is implementation-defined

---

## automotive-avoid-signal-header

Detects inclusion of `<signal.h>` and signal functions.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 21.5 |
| **Category** | Required |
| **Functions** | signal, raise |

### Rationale

Signal handlers have severe restrictions:
- Limited to async-signal-safe functions
- Race conditions with main program
- Difficult to test and verify

---

## automotive-avoid-ascii-to-number

Detects usage of atoi, atof, atol, atoll.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 21.7 |
| **Category** | Required |
| **Functions** | atoi, atof, atol, atoll |

### Rationale

These functions have undefined behavior on error:
- Cannot distinguish 0 from error
- No range checking
- No error reporting

{: .note }
> **Alternative:** Use strtol, strtod with error checking

### Example

{: .warning }
> **Violation**

```c
int value = atoi("123");  // Warning
```

{: .note }
> **Compliant**

```c
char *endptr;
long value = strtol("123", &endptr, 10);
if (*endptr != '\0') {
    // Handle error
}
```

---

## automotive-avoid-stdlib-exit

Detects usage of exit, abort, _Exit.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 21.8 |
| **Category** | Required |
| **Functions** | exit, abort, _Exit |

### Rationale

Abrupt termination:
- Bypasses normal cleanup
- May leave hardware in undefined state
- Not suitable for safety-critical systems

---

## automotive-avoid-stdlib-system

Detects usage of the system() function.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 21.8 |
| **Category** | Required |
| **Functions** | system |

### Rationale

Security and predictability risks:
- Command injection vulnerabilities
- Environment-dependent behavior
- Not analyzable statically

---

## automotive-avoid-stdlib-rand

Detects usage of rand() and srand().

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 21.24 |
| **Category** | Required |
| **Functions** | rand, srand |

### Rationale

- Pseudorandom, predictable sequence
- Quality is implementation-defined
- Not suitable for security purposes

---

## automotive-avoid-stdarg-header

Detects inclusion of `<stdarg.h>`.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 17.1 |
| **Category** | Required |

### Rationale

Variable argument functions bypass type safety:
- No compile-time type checking
- Buffer overflow risks
- Difficult to maintain

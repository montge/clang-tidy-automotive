---
layout: default
title: Preprocessor Checks
parent: Checks
nav_order: 4
---

# Preprocessor Checks
{: .no_toc }

Checks for preprocessor rules (MISRA C:2025 Rules 20.x).
{: .fs-6 .fw-300 }

## Table of Contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## automotive-avoid-macro-named-as-ckeyword

Detects macro names that match C keywords.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 20.4 |
| **Category** | Required |

### Example

{: .warning }
> **Violation**

```c
#define if 0      // Warning: redefining keyword
#define while(x)  // Warning: redefining keyword
```

---

## automotive-avoid-undef

Detects usage of the #undef directive.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 20.5 |
| **Category** | Advisory |

### Rationale

#undef can cause confusion about macro state:
- Macro availability becomes location-dependent
- Harder to analyze preprocessor behavior

### Example

{: .warning }
> **Violation**

```c
#define MAX 100
// ...
#undef MAX  // Warning: #undef usage
#define MAX 200
```

---

## automotive-avoid-hash-operator

Detects usage of the # stringification operator.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 20.10 |
| **Category** | Advisory |

### Rationale

Stringification obscures what code actually does:
- Debugging is harder
- Code review more difficult

### Example

{: .warning }
> **Violation**

```c
#define STR(x) #x  // Warning: # operator

STR(hello)  // Expands to "hello"
```

---

## automotive-avoid-multiple-hash-operators

Detects usage of the ## token concatenation operator.

| Property | Value |
|:---------|:------|
| **MISRA Rule** | 20.11 |
| **Category** | Advisory |

### Rationale

Token pasting creates identifiers that:
- Can't be found with text search
- Are harder to understand
- May have unexpected behavior

### Example

{: .warning }
> **Violation**

```c
#define CONCAT(a, b) a##b  // Warning: ## operator

int CONCAT(my, Var);  // Creates myVar
```

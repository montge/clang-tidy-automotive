# MISRA Implementation Gap Analysis

This document provides a comprehensive gap analysis between the MISRA requirements and the clang-tidy-automotive implementation.

**Generated:** 2025-12-21

## Executive Summary

| Standard | Total Rules/Dirs | Implemented | Clang Built-in | Manual Review | Missing |
|----------|------------------|-------------|----------------|---------------|---------|
| MISRA C:2025 | 197 | 116 | 10 | 17 dirs + 14 rules | 49 rules, 2 dirs |
| MISRA C++:2023 | 67 | 22 | 0 | TBD | 45 rules, 2 dirs |

**Recent Additions (Phase 1 Quick Wins - December 2025):**
- MISRA C:2025: 12 new checks (7.2, 7.4, 7.6, 8.18, 11.10, 11.11, 16.6, 17.10, 17.13, 20.14, 21.9, 21.24)
- MISRA C++:2023: 6 new checks (9.3, 9.4, 12.3, 18.1, 21.6, 21.10)

---

## MISRA C:2025 Gap Analysis

### Missing Mandatory Rules (9 rules) - HIGHEST PRIORITY

These MUST be implemented for compliance:

| Rule | Title | Decidability | Notes |
|------|-------|--------------|-------|
| **7.5** | The argument of an integer constant macro shall have an appropriate form | Decidable | Macro argument validation |
| **9.7** | Atomic objects shall be appropriately initialized before being accessed | Undecidable | Requires dataflow analysis |
| **12.5** | The sizeof operator shall not have an operand which is a function parameter declared as 'array of type' | Decidable | Array-to-pointer decay in sizeof |
| **17.9** | A function declared with a _Noreturn function specifier shall not return to its caller | Undecidable | Control flow analysis |
| **18.10** | Pointers to variably-modified array types shall not be used | Decidable | VLA pointer restrictions |
| **19.1** | An object shall not be assigned or copied to an overlapping object | Undecidable | Aliasing analysis |
| **21.18** | The size_t argument passed to any function in <string.h> shall have an appropriate value | Undecidable | Buffer size validation |
| **21.22** | All operand arguments to type-generic macros in <tgmath.h> shall have an appropriate essential type | Decidable | Type checking |
| **22.20** | Thread-specific storage pointers shall be created before being accessed | Undecidable | Thread lifecycle analysis |

### Missing Required Rules (35 rules) - HIGH PRIORITY

| Rule | Title | Decidability | Complexity |
|------|-------|--------------|------------|
| **1.5** | Obsolescent language features shall not be used | Undecidable | Medium |
| **2.2** | A project shall not contain dead code | Undecidable | High |
| **6.3** | A bit-field shall not be declared as a member of a union | Decidable | Low |
| **8.6** | An identifier with external linkage shall have exactly one external definition | Decidable | Medium |
| **9.6** | An initializer using chained designators shall not contain initializers without designators | Decidable | Low |
| **12.6** | Structure and union members of atomic objects shall not be directly accessed | Decidable | Low |
| **14.2** | A for loop shall be well-formed | Undecidable | Medium |
| **16.1** | All switch statements shall be well-formed | Decidable | Medium |
| **18.9** | An object with temporary lifetime shall not undergo array-to-pointer conversion | Decidable | Medium |
| **19.3** | A union member shall not be read unless it has been previously set | Undecidable | High |
| **20.6** | Tokens that look like a preprocessing directive shall not occur within a macro argument | Decidable | Low |
| **20.7** | Expressions resulting from macro expansion shall be appropriately delimited | Decidable | Medium |
| **20.8** | The controlling expression of #if/#elif shall evaluate to 0 or 1 | Decidable | Low |
| **20.9** | All identifiers in #if/#elif shall be #define'd before evaluation | Decidable | Low |
| **20.13** | A line whose first token is # shall be a valid preprocessing directive | Decidable | Low |
| **20.15** | #define and #undef shall not be used on a reserved identifier or macro name | Decidable | Low |
| **21.14** | memcmp shall not be used to compare null terminated strings | Undecidable | Medium |
| **21.23** | All operand arguments to multi-argument type-generic macros shall have same standard type | Decidable | Medium |
| **21.25** | All memory synchronization operations shall be executed in sequentially consistent order | Decidable | Medium |
| **21.26** | mtx_timedlock() shall only be invoked on mutex objects of appropriate type | Undecidable | High |
| **22.11** | A previously joined/detached thread shall not be subsequently joined/detached | Undecidable | High |
| **22.13** | Thread objects/sync objects/TSS pointers shall have appropriate storage duration | Decidable | Medium |
| **22.15** | Thread sync objects/TSS pointers shall not be destroyed until all threads have terminated | Undecidable | High |
| **22.16** | All mutex objects locked by a thread shall be explicitly unlocked by same thread | Undecidable | High |
| **22.17** | No thread shall unlock mutex not previously locked by it | Undecidable | High |
| **22.18** | Non-recursive mutexes shall not be recursively locked | Undecidable | High |
| **22.19** | A condition variable shall be associated with at most one mutex object | Undecidable | High |
| **23.4** | A generic association shall list an appropriate type | Decidable | Low |
| **23.6** | Controlling expression of generic selection shall have matching essential/standard type | Decidable | Low |
| **23.8** | A default association shall appear first or last in a generic selection | Decidable | Low |

### Missing Advisory Rules (10 rules)

| Rule | Title | Decidability |
|------|-------|--------------|
| **8.13** | A pointer should point to a const-qualified type whenever possible | Undecidable |
| **8.16** | The alignment specification of zero should not appear in an object declaration | Decidable |
| **8.17** | At most one explicit alignment specifier should appear in an object declaration | Decidable |
| **8.19** | There should be no external declarations in a source file | Decidable |
| **17.11** | A function that never returns should be declared with _Noreturn | Undecidable |
| **17.12** | A function identifier should only be used with & or parenthesized parameter list | Decidable |
| **23.1** | A generic selection should only be expanded from a macro | Decidable |
| **23.3** | A generic selection should contain at least one non-default association | Decidable |
| **23.5** | A generic selection should not depend on implicit pointer type conversion | Decidable |
| **23.7** | A generic selection expanded from macro should evaluate argument only once | Decidable |

### Missing Directives (2)

| Directive | Title | Category |
|-----------|-------|----------|
| **Dir 1.2** | The use of language extensions should be minimized | Advisory |
| **Dir 4.2** | All usage of assembly language should be documented | Advisory |

---

## MISRA C++:2023 Gap Analysis

### Missing Mandatory Rules (1 rule) - HIGHEST PRIORITY

| Rule | Title | Decidability |
|------|-------|--------------|
| **25.5** | The pointers returned by localeconv, getenv, setlocale, strerror must only be used as pointer to const-qualified type | Decidable |

### Missing Required Rules (34 rules) - HIGH PRIORITY

| Rule | Title | Decidability |
|------|-------|--------------|
| **0.1** | A variable shall not be implicitly captured in a lambda | Decidable |
| **0.3** | Unknown | Decidable |
| **4.6** | Memory used for synchronization shall be appropriately sequenced | Undecidable |
| **5.7** | An identifier shall be declared within the smallest possible scope | Decidable |
| **5.10** | An identifier with array type shall not be used in an expression with pointer semantics | Decidable |
| **5.13** | An identifier should not have both internal and external linkage | Decidable |
| **6.2** | An identifier declared in a local scope shall not have the same name as an identifier in an enclosing scope | Decidable |
| **6.4** | Assignment operators shall not be used in sub-expressions | Decidable |
| **6.7** | Floating-point expressions shall not be tested for equality or inequality | Decidable |
| **7.0** | A value of any of the standard character types shall only be assigned to a variable of the same character type | Decidable |
| **7.11** | A literal value shall not be assigned to a null pointer constant | Decidable |
| **8.7** | Pointer arithmetic shall not be applied to a pointer to object of non-array type | Undecidable |
| **9.2** | Initialization of an auto variable shall happen at its declaration | Decidable |
| **9.5** | A switch label shall only be used in the outermost compound statement of a switch body | Decidable |
| **9.6** | All paths that can return values shall return a value | Decidable |
| **10.1** | The value of an expression shall not be assigned to an object of a different essential type | Decidable |
| **10.2** | An expression containing arithmetic operators of the same essential type shall not be implicitly converted to a different essential type | Decidable |
| **10.4** | The value of a composite expression shall not be assigned to a narrower essential type | Decidable |
| **11.6** | A pointer to member shall not be converted to a pointer to another member type | Decidable |
| **12.2** | Subtraction between two pointers shall only be performed when they are pointing to elements of the same array | Undecidable |
| **13.1** | An object shall not be copied or moved to an overlapping object | Undecidable |
| **13.3** | A parameter passed by value shall not be modified | Decidable |
| **16.5** | A class should only define an overloaded operator && or operator || if it does not rely on short-circuit evaluation | Decidable |
| **17.8** | Virtual functions shall not be called during construction or destruction | Undecidable |
| **18.3** | A move or copy constructor shall not leave the original object in an invalid state | Undecidable |
| **19.1** | An object shall not undergo type-punning | Decidable |
| **19.2** | A reference or pointer to a derived class shall not be implicitly cast to a reference or pointer to a base class | Decidable |
| **19.3** | The <cstdlib> function realloc shall not be used | Decidable |
| **21.2** | A pointer to a file shall have a valid value when used | Undecidable |
| **22.3** | A pointer object shall have a value that points to an object | Undecidable |
| **22.4** | A pointer object shall have a value that is within the bounds of an array object | Undecidable |
| **28.3** | A function with side effects shall not be called from a context where those side effects would be discarded | Undecidable |
| **28.6** | A function call site shall be compatible with the function signature | Decidable |
| **30.0** | Standard library stream objects shall not be used while in a fail state | Undecidable |

### Missing Advisory Rules (21 rules)

| Rule | Title | Decidability |
|------|-------|--------------|
| **0.0** | A controlling expression shall not be invariant | Undecidable |
| **0.2** | A project shall not contain infeasible paths | Undecidable |
| **4.1** | The value of a standard character shall only be compared to a character literal of the same type | Decidable |
| **5.0** | A type-specifier-seq or type-id shall not define any types | Decidable |
| **5.7** (Adv) | A tag name should not hide other identifiers | Decidable |
| **6.0** | An array object should be explicitly initialized | Decidable |
| **6.5** | A loop counter shall not be of floating type | Decidable |
| **6.8** | A function should not contain sub-optimal use of break or continue statements | Decidable |
| **6.9** | An object shall only be declared in a block if that block always uses the object | Undecidable |
| **8.0** | A function should have a single point of exit | Decidable |
| **8.1** | All variables captured by a lambda expression should be explicitly listed in the capture list | Decidable |
| **8.18** | The statement expression extension to C should not be used | Decidable |
| **8.19** | A noreturn function shall not return | Decidable |
| **8.20** | A noreturn function should have void return type | Decidable |
| **10.3** | An unnamed namespace should be used to define entities with internal linkage | Decidable |
| **11.3** | A C-style cast should not be performed | Decidable |
| **14.1** | A variable or non-static data member should only be declared as constexpr if it has no mutable sub-objects | Decidable |
| **16.6** | A non-type template parameter of reference type should not bind to a temporary | Decidable |
| **18.5** | An exception-specification shall not be dynamic | Decidable |
| **19.0** | std::addressof should be used to obtain the address of an object | Decidable |
| **26.3** | Every module interface or partition shall be imported by every other partition of the same module | Decidable |

### Missing Directives (2)

| Directive | Title | Category |
|-----------|-------|----------|
| **Dir 0.3** | Unknown | Required |
| **Dir 5.7** | Unknown | Required |

---

## Implementation Priority Recommendations

### Phase 1: Quick Wins (Decidable, Low Complexity)
Focus on rules that are decidable and can be implemented with simple AST matching:

**MISRA C:2025 (12 implemented, 3 deferred):**
1. ~~Rule 7.2 - Unsigned literal suffix~~ ✓ IMPLEMENTED
2. ~~Rule 7.4 - String literal const assignment~~ ✓ IMPLEMENTED
3. ~~Rule 7.6 - Small integer constant macros~~ ✓ IMPLEMENTED
4. ~~Rule 8.18 - Tentative definitions in header~~ ✓ IMPLEMENTED
5. ~~Rule 11.10 - Atomic void restriction~~ ✓ IMPLEMENTED
6. ~~Rule 11.11 - Implicit NULL comparison~~ ✓ IMPLEMENTED
7. ~~Rule 16.6 - Switch minimum clauses~~ ✓ IMPLEMENTED
8. ~~Rule 17.10 - Noreturn void return type~~ ✓ IMPLEMENTED
9. ~~Rule 17.13 - Function type qualification~~ ✓ IMPLEMENTED
10. ~~Rule 20.14 - Preprocessor directive file scope~~ ✓ IMPLEMENTED
11. ~~Rule 21.9 - bsearch/qsort prohibition~~ ✓ IMPLEMENTED
12. ~~Rule 21.24 - Random number function prohibition~~ ✓ IMPLEMENTED
13. Rule 6.3 - Bit-field in union (covered by `automotive-avoid-bitfield-in-union`)
14. Rule 20.8 - #if expression value (Deferred - complex)
15. Rule 20.9 - #if undefined identifiers (Deferred - complex)
16. Rule 20.13 - Valid preprocessing directive (Deferred - Clang handles)

**MISRA C++:2023 (6 implemented - COMPLETE):**
1. ~~Rule 9.3 - Compound statement body~~ ✓ IMPLEMENTED
2. ~~Rule 9.4 - Switch minimum clauses~~ ✓ IMPLEMENTED
3. ~~Rule 12.3 - Union prohibition~~ ✓ IMPLEMENTED
4. ~~Rule 18.1 - Exception prohibition~~ ✓ IMPLEMENTED
5. ~~Rule 21.6 - Dynamic memory prohibition~~ ✓ IMPLEMENTED
6. ~~Rule 21.10 - <ctime> prohibition~~ ✓ IMPLEMENTED

### Phase 2: Medium Complexity
Rules requiring more sophisticated analysis:

- Rule 1.5 - Obsolescent features
- Rule 16.1 - Well-formed switch
- Rule 14.2 - Well-formed for loop
- Rule 20.7 - Macro expansion parentheses
- Essential type checks for generics

### Phase 3: Advanced Analysis
Rules requiring dataflow, aliasing, or thread analysis:

- Rule 2.2 - Dead code detection
- Rule 9.7 - Atomic initialization
- Rule 19.1/19.3 - Overlapping storage
- Rule 22.x - Thread safety rules

---

## Coverage Statistics by Category

### MISRA C:2025 Coverage

| Category | Total | Implemented | Gap |
|----------|-------|-------------|-----|
| 1.x Language | 2 | 1 (1.2) | 1.5 |
| 2.x Unused | 8 | 6 | 2.2 |
| 3.x Comments | 2 | 2 | 0 |
| 4.x Chars | 2 | 2 | 0 |
| 5.x Identifiers | 9 | 3 | 6* |
| 6.x Types | 3 | 2 | 6.3 |
| 7.x Literals | 6 | 4 | 7.5, 7.6 |
| 8.x Declarations | 15 | 8 | 7 |
| 9.x Initialization | 7 | 5 | 9.6, 9.7 |
| 10.x Essential types | 7 | 6 | 0** |
| 11.x Pointers | 10 | 9 | 1 |
| 12.x Expressions | 4 | 3 | 12.5, 12.6 |
| 13.x Side effects | 4 | 4 | 0 |
| 14.x Control flow | 4 | 3 | 14.2 |
| 15.x Control statements | 7 | 7 | 0 |
| 16.x Switch | 7 | 6 | 16.1 |
| 17.x Functions | 11 | 8 | 3 |
| 18.x Pointers/arrays | 8 | 6 | 18.9, 18.10 |
| 19.x Overlapping storage | 3 | 1 | 19.1, 19.3 |
| 20.x Preprocessing | 14 | 8 | 6 |
| 21.x Standard library | 16 | 11 | 5 |
| 22.x Resources | 19 | 1 | 18*** |
| 23.x Generics | 7 | 0 | 7 |

*5.x rules largely require manual review
**10.3 covered by clang built-in
***22.x is a major gap area - thread safety rules

### MISRA C++:2023 Coverage

| Category | Total | Implemented | Gap |
|----------|-------|-------------|-----|
| 0.x | 3 | 0 | 3 |
| 4.x | 2 | 0 | 2 |
| 5.x | 4 | 0 | 4 |
| 6.x | 8 | 0 | 8 |
| 7.x | 2 | 0 | 2 |
| 8.x | 8 | 5 | 3 |
| 9.x | 5 | 2 | 3 |
| 10.x | 4 | 0 | 4 |
| 11.x | 2 | 0 | 2 |
| 12.x | 2 | 1 | 1 |
| 13.x | 2 | 0 | 2 |
| 14.x | 1 | 0 | 1 |
| 15.x | 2 | 2 | 0 |
| 16.x | 2 | 0 | 2 |
| 17.x | 1 | 0 | 1 |
| 18.x | 4 | 4 | 0 |
| 19.x | 4 | 0 | 4 |
| 21.x | 3 | 2 | 1 |
| 22.x | 2 | 0 | 2 |
| 25.x | 1 | 0 | 1 |
| 26.x | 1 | 0 | 1 |
| 28.x | 2 | 0 | 2 |
| 30.x | 1 | 0 | 1 |

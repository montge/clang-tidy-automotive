# MISRA C:2025 Examples

This directory contains example code demonstrating MISRA C:2025 rule violations and compliant alternatives.

## Directory Structure

```
examples/
├── directives/           # MISRA Directives (Dir 1.x - 5.x)
│   ├── dir-1/           # Implementation Environment
│   ├── dir-2/           # Language Extensions
│   ├── dir-4/           # Code Design
│   └── dir-5/           # Libraries
│
└── rules/               # MISRA Rules (Rule 1.x - 23.x)
    ├── rule-1/          # Standard C Environment
    ├── rule-2/          # Unused Code
    ├── rule-3/          # Comments
    ├── rule-4/          # Character Sets
    ├── rule-5/          # Identifiers
    ├── rule-6/          # Types
    ├── rule-7/          # Literals
    ├── rule-8/          # Declarations
    ├── rule-9/          # Initialization
    ├── rule-10/         # Essential Type Model
    ├── rule-11/         # Pointer Type Conversions
    ├── rule-12/         # Expressions
    ├── rule-13/         # Side Effects
    ├── rule-14/         # Control Statement Expressions
    ├── rule-15/         # Control Flow
    ├── rule-16/         # Switch Statements
    ├── rule-17/         # Functions
    ├── rule-18/         # Pointers and Arrays
    ├── rule-19/         # Overlapping Storage
    ├── rule-20/         # Preprocessing Directives
    ├── rule-21/         # Standard Libraries
    ├── rule-22/         # Resources
    └── rule-23/         # Atomic and Concurrency
```

## File Naming Convention

Each example file follows this naming pattern:

```
<rule-id>-<short-name>-<violation|compliant>.c
```

Examples:
- `15.1-goto-violation.c` - Code that violates Rule 15.1
- `15.1-goto-compliant.c` - Compliant alternative
- `21.3-malloc-violation.c` - Code that violates Rule 21.3
- `21.3-malloc-compliant.c` - Compliant alternative

## Example Format

Each example file should contain:

1. Header comment with rule reference
2. Brief description of the violation/compliance
3. Minimal code demonstrating the point
4. Expected clang-tidy output (for violations)

### Violation Example Template

```c
// MISRA C:2025 Rule X.Y - <Rule Title>
// Category: <Required|Advisory|Mandatory>
// Status: VIOLATION
//
// This code violates Rule X.Y because <reason>.
//
// Expected diagnostic:
//   warning: <check message> [automotive-check-name]

<code demonstrating violation>
```

### Compliant Example Template

```c
// MISRA C:2025 Rule X.Y - <Rule Title>
// Category: <Required|Advisory|Mandatory>
// Status: COMPLIANT
//
// This code complies with Rule X.Y by <explanation>.

<compliant code>
```

## Running Examples

To verify examples with clang-tidy:

```bash
# Check a single file
clang-tidy -checks='-*,automotive-*' examples/rules/rule-15/15.1-goto-violation.c

# Check all examples
find examples -name '*-violation.c' -exec clang-tidy -checks='-*,automotive-*' {} \;
```

## Contributing Examples

When adding new examples:

1. Place in the appropriate rule category directory
2. Follow the naming convention
3. Include both violation and compliant versions
4. Verify the example triggers the expected diagnostic
5. Keep examples minimal and focused

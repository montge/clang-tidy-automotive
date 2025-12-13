# Test Directory Structure and Conventions

This document describes the testing conventions for clang-tidy-automotive checks.

## Directory Structure

```
test/
├── README.md                 # This file
├── checkers/
│   └── automotive/           # Main test directory
│       ├── array/            # Array-related check tests
│       ├── bitfield/         # Bit-field check tests
│       ├── c2023/            # MISRA C:2023 organized tests
│       │   ├── {rule}/       # e.g., 2.3/, 8.14/, 21.5/
│       ├── comment/          # Comment check tests
│       ├── expression/       # Expression check tests
│       ├── function/         # Function check tests
│       ├── literal/          # Literal check tests
│       ├── operator/         # Operator check tests
│       ├── pointer/          # Pointer check tests
│       ├── preprocessor/     # Preprocessor check tests
│       ├── return/           # Return statement check tests
│       ├── statement/        # Statement check tests
│       ├── stdlib/           # Standard library check tests
│       ├── storage/          # Storage class check tests
│       └── type/             # Type check tests
└── lit.cfg.py               # LIT test configuration (if applicable)
```

## File Naming Conventions

### Pattern
```
{check-name-without-automotive-prefix}.c
```

### Examples
- `automotive-avoid-goto` → `avoid-goto.c`
- `automotive-missing-default-in-switch` → `missing-default-in-switch.c`
- `automotive-c23-req-14.1` → `c2023/14.1/float-loop-counter.c`

### Multiple Tests for Same Check
When a check requires multiple test scenarios:
```
{check-name}.c                    # Primary test
{check-name}-edge-cases.c         # Edge case testing
{check-name}-compliant.c          # Negative test (code that should NOT trigger)
```

## Test File Structure

Each test file should follow this structure:

```c
// Test file for: automotive-{check-name}
// Related MISRA C:2025 Rule: {rule-number}
//
// This file tests the detection of {brief description of what is detected}

// RUN: %check_clang_tidy %s automotive-{check-name} %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_basic_violation(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: {expected warning message}
    violation_code();
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_edge_case_1(void) {
    // Description of edge case
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: {expected warning message}
    edge_case_code();
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_code(void) {
    // This should not trigger any warning
    compliant_code();
}
```

## Check-to-Test Mapping

| Check ID | Test File(s) | Status |
|----------|--------------|--------|
| `automotive-avoid-goto` | `statement/avoid-goto.c` | ✅ |
| `automotive-avoid-flexible-array-member` | `array/avoid-flexible-array-member.c` | ✅ |
| `automotive-avoid-bitfield-in-union` | `bitfield/avoid-bitfield-in-union.c` | ❌ Missing |
| `automotive-avoid-signed-single-bitfield` | `bitfield/avoid-signed-single-bitfield.c` | ❌ Missing |
| ... | ... | ... |

## Running Tests

### Run All Tests
```bash
cd build
ninja check-clang-tools
```

### Run Specific Test
```bash
./bin/clang-tidy test/checkers/automotive/statement/avoid-goto.c --checks="automotive-*" --
```

### Run Tests with Verbose Output
```bash
./bin/clang-tidy test/checkers/automotive/statement/avoid-goto.c \
    --checks="automotive-*" \
    --extra-arg="-std=c11" \
    --
```

## Coverage Collection

See `scripts/coverage.sh` for collecting code coverage data from test runs.

## Adding New Tests

1. Create test file in appropriate directory
2. Follow the file structure template above
3. Include both violation and compliant cases
4. Add CHECK-MESSAGES directives for expected warnings
5. Update this README's mapping table
6. Run test to verify it works

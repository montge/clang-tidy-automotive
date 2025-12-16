# Code Coverage

This document describes how to generate and view code coverage reports for clang-tidy-automotive.

## Overview

Code coverage measures how much of the clang-tidy-automotive source code is exercised by the test suite. This helps identify untested code paths and ensures comprehensive testing.

**Current Coverage: 83.89% line coverage** (230 files, 2558 lines)

## Local-Only Coverage

**Important:** Code coverage can only be generated locally, not in CI/CD pipelines.

This is because coverage instrumentation requires rebuilding the entire LLVM/Clang project with special compiler flags, which:
- Requires ~15GB of disk space for the build
- Takes ~2 hours on typical hardware
- Is too resource-intensive for GitHub Actions free tier

## Prerequisites

Before generating coverage, ensure you have:

1. **LLVM source downloaded**: Run `./download.sh` if not already done
2. **Symlinks set up**: Run `./setup.sh` if not already done
3. **LLVM coverage tools installed**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install llvm-20

   # Or use your distribution's package manager
   ```

## Generating Coverage Reports

### Step 1: Configure Coverage Build

Configure the build with coverage instrumentation:

```bash
./scripts/configure-coverage.sh
```

This adds `-fprofile-instr-generate -fcoverage-mapping` flags to the build.

### Step 2: Build with Coverage

Build clang-tidy with coverage instrumentation:

```bash
ninja -C build clang-tidy
```

**Note:** This rebuilds the entire project with coverage flags enabled. This is a one-time step unless you change the configuration.

### Step 3: Run Coverage Collection

Run the test suite and generate coverage reports:

```bash
# Generate text summary
./scripts/coverage.sh --report

# Generate HTML report (optional)
./scripts/coverage.sh --html
```

### Step 4: View Results

Coverage reports are generated in the `coverage/` directory:

| File | Description |
|------|-------------|
| `coverage-summary.txt` | Text summary with per-file coverage |
| `coverage.lcov` | LCOV format for tools like lcov-viewer |
| `coverage-show.txt` | llvm-cov show format for SonarCloud |
| `html/index.html` | Interactive HTML report (if --html used) |

## Uploading to SonarCloud

To upload coverage data to SonarCloud:

```bash
./scripts/upload-coverage-to-sonarcloud.sh
```

This script:
1. Commits the coverage data to the repository
2. Triggers SonarCloud analysis with coverage data

## Coverage Targets

| Metric | Target | Current |
|--------|--------|---------|
| Line Coverage | 90% | 83.89% |
| Function Coverage | 100% | 100% |
| Branch Coverage | 80% | ~70% |

## Coverage Gaps

The following checks need improved test coverage:

### Checks with <50% Coverage

| Check | Coverage | MISRA Rule | Issue |
|-------|----------|------------|-------|
| ImplicitFunctionDeclCheck | 0% | 8.1 | Test uses wrong check ID |
| PreprocessorFlowCheck | 0% | - | Empty stub (not implemented) |
| MissingStaticForInternalCheck | 3% | 8.7 | Tests not triggering check |
| MissingStaticInternalLinkageCheck | 33% | 8.7 | Needs more test cases |
| AvoidCompositeExpressionMismatchCheck | 38% | 10.x | Needs expression tests |
| WrongNullPointerValueCheck | 40% | 11.9 | Needs null pointer tests |
| VirtualDestructorCheck | 44% | C++ 15.7 | Needs more C++ edge cases |
| AvoidAutoAddressEscapeCheck | 47% | 18.6 | Needs escape scenarios |

### Checks with 50-70% Coverage

| Check | Coverage | Notes |
|-------|----------|-------|
| AvoidPointerTypedefCheck | 52% | Dir 4.8 |
| AvoidInvalidHeaderCharCheck | 58% | 20.2 |
| AvoidLinesplicingWithinCommentCheck | 65% | 3.2 |
| AvoidLanguageExtensionCheck | 68% | 1.2 |
| MissingHeaderGuardCheck | 69% | Dir 4.10 |

### MISRA C++ Coverage

All 5 MISRA C++:2023 checks have test files:
- avoid-c-style-cast.cpp (8.2.1)
- explicit-constructor.cpp (15.1.3)
- avoid-slicing.cpp (15.3)
- virtual-destructor.cpp (15.7) - needs more edge cases
- noexcept-move.cpp (18.4.2)

## Improving Coverage

To improve coverage:

1. Identify uncovered code paths:
   ```bash
   grep "0|" coverage/coverage-show.txt | head -20
   ```

2. Add tests targeting those paths in `test/checkers/automotive/`

3. Re-run coverage to verify improvement

## CI Integration

While coverage cannot be generated in CI, the committed `coverage/coverage.lcov` file is used by SonarCloud during PR analysis. This provides visibility into coverage changes without requiring a full rebuild in CI.

The coverage workflow (`.github/workflows/coverage.yml`) is available for manual triggering on self-hosted runners with sufficient resources.

## Troubleshooting

### "No profile data files found"
The build wasn't configured with coverage flags. Run `./scripts/configure-coverage.sh` and rebuild.

### "llvm-profdata not found"
Install LLVM tools: `sudo apt-get install llvm-20`

### Low coverage percentage
Ensure you're running all test files. Check that the test files exercise the code paths you expect.

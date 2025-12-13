## Context
The clang-tidy-automotive project aims to provide MISRA C:2025 compliance checking that can:
1. Integrate with upstream LLVM clang-tidy (potential upstreaming)
2. Work with LTS-supported compiler versions (Clang 17, 18, 19, 20)
3. Report compliance via SonarQube for enterprise workflows
4. Provide comprehensive test coverage for safety-critical use cases

The current implementation has ~55 checks with inconsistent naming, limited test coverage, and no formal coverage measurement.

## Goals / Non-Goals

### Goals
- Support Clang/LLVM versions under LTS support (17.x, 18.x, 19.x, 20.x)
- Structure code to enable potential upstream contribution to LLVM
- Achieve 90%+ test coverage with measurable metrics
- Enable SonarQube integration for MISRA compliance reporting
- Provide examples demonstrating each rule for educational/testing purposes
- Self-documenting code with Doxygen

### Non-Goals
- C++ MISRA support (future phase, separate proposal)
- GUI tooling for rule configuration
- Custom IDE integrations (rely on standard clang-tidy integration)
- Reproducing MISRA rule text (copyright restriction)

## Decisions

### Decision: Check ID Naming Convention
**What:** Standardize check IDs to `automotive-c25-{category}-{rule.subrule}` format
**Why:** Enables clear mapping to MISRA rules, consistent with upstream clang-tidy naming patterns
**Alternatives considered:**
- Keep descriptive names (`automotive-avoid-goto`): Rejected - harder to map to compliance requirements
- Use MISRA rule numbers only (`misra-c2025-15.1`): Rejected - copyright concerns with "MISRA" prefix

### Decision: Multi-Version LLVM Support
**What:** Support Clang 17-20 via conditional compilation where needed
**Why:** Enterprise users may be locked to LTS versions
**Implementation:**
```cpp
#if LLVM_VERSION_MAJOR >= 18
  // Use newer API
#else
  // Use legacy API
#endif
```

### Decision: Coverage Infrastructure
**What:** Use llvm-cov with custom scripts for coverage measurement
**Why:** Native to LLVM toolchain, integrates with existing build system
**Alternatives considered:**
- gcov: Works but less integrated with LLVM
- Codecov.io: Adds external dependency

### Decision: SonarQube Integration via SARIF
**What:** Export clang-tidy results as SARIF format, import to SonarQube via generic issue import
**Why:** SARIF is industry standard, avoids custom SonarQube plugin maintenance
**Alternatives considered:**
- Custom SonarQube plugin: High maintenance burden
- Direct JSON export: Not standardized

### Decision: Dual SonarQube Strategy
**What:** Two distinct SonarQube use cases:
1. **Project Quality:** Use SonarQube Cloud to evaluate clang-tidy-automotive codebase quality (C++ code quality, test coverage, security)
2. **MISRA Reporting:** Enable users to import clang-tidy-automotive results into their SonarQube (on-premise or Cloud) for MISRA compliance reporting

**Why:** Separates internal quality assurance from the product feature
**Implementation:**
- Configure `sonar-project.properties` for this repository's C++ analysis
- Build SARIF export capability for MISRA compliance reporting to any SonarQube instance

### Decision: SonarQube Version Support
**What:** Support SonarQube LTS versions for on-premise deployments
**Target versions:**
- SonarQube 9.9 LTS (current LTS as of 2024)
- SonarQube 10.x (latest)
- SonarCloud (always current)
**Why:** Enterprise users often run LTS versions for stability

### Decision: Examples Directory Structure
**What:** Organize examples by rule category with `violation/` and `compliant/` subdirectories
**Structure:**
```
examples/
  misra-c-2025/
    directives/
      dir-1-1/
        violation.c
        compliant.c
    rules/
      rule-15-1/
        violation.c
        compliant.c
```
**Why:** Clear organization, easy to navigate, supports automated testing

## Risks / Trade-offs

### Risk: Breaking existing users with ID changes
**Mitigation:**
- Document migration path clearly
- Consider providing ID aliases during transition period
- Update all documentation and examples

### Risk: Multi-version support complexity
**Mitigation:**
- Use compile-time conditionals only where API changes
- Test on all supported versions in CI
- Document version-specific behavior

### Risk: Coverage measurement accuracy
**Mitigation:**
- Exclude test files from coverage metrics
- Use branch coverage, not just line coverage
- Regular coverage audits

## Migration Plan

### Phase 1: Non-breaking additions
1. Add Doxygen documentation (no behavior change)
2. Add examples directory (no behavior change)
3. Add coverage tooling (no behavior change)

### Phase 2: ID standardization (breaking)
1. Add new IDs as aliases alongside old IDs
2. Deprecation warnings for old IDs (1 release cycle)
3. Remove old IDs

### Phase 3: New checks
1. Implement in priority order (Required > Advisory)
2. Each check includes tests and examples

## Resolved Questions
- **Clang 16.x support:** No - focus on 17+ (current LTS versions)
- **SonarQube target:** Support both on-premise and SonarQube Cloud
- **Examples scope:** C only for this phase (C++ is future work)

## Open Questions
- None currently

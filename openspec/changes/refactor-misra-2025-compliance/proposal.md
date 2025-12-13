## Why
The current clang-tidy-automotive implementation covers only a subset of MISRA rules with inconsistent naming and limited test coverage (~31 test files for ~55 checks). To provide reliable MISRA C:2025 compliance checking for automotive software, we need comprehensive rule coverage, 90%+ test coverage, and integration with SonarQube for compliance reporting.

## What Changes
- **BREAKING**: Standardize check registration IDs to follow `automotive-c25-{req|adv|man|dir}-{rule.subrule}` pattern
- Add missing MISRA C:2025 rules and directives (currently ~55 checks, target 150+)
- Create comprehensive `examples/` directory with violation and compliant code for each rule
- Implement test infrastructure for 90%+ code coverage measurement
- Add SonarQube integration for MISRA compliance reporting
- Add Doxygen documentation to all check classes
- Implement consistent error handling and diagnostic messages
- Add security review and linting infrastructure

## Impact
- Affected specs: rule-coverage, test-infrastructure, examples, sonarqube-integration, code-quality
- Affected code:
  - `src/automotive/**/*.cpp` - All check implementations
  - `src/automotive/**/*Component.cpp` - Check registration (ID changes)
  - `test/checkers/automotive/**` - All test files
  - `examples/` - New directory
  - `CMakeLists.txt` files - Build configuration updates
  - `.clang-tidy` - Configuration updates

## Phases
1. **Phase 1: Code Quality Foundation** - Doxygen, linting, formatting, security review
2. **Phase 2: Test Infrastructure** - Coverage tooling, test organization, baseline coverage
3. **Phase 3: Examples Directory** - Violation/compliant examples for all rules
4. **Phase 4: Rule Coverage Expansion** - Implement missing MISRA C:2025 checks
5. **Phase 5: SonarQube Integration** - Compliance reporting and rule mapping

## Success Criteria
- All existing checks pass with updated IDs
- 90%+ code coverage achieved
- All MISRA C:2025 rules have either: implemented check, mapped to existing clang check, or documented as manual-review-only
- SonarQube can consume and report MISRA compliance
- All code has Doxygen documentation

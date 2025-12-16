## 1. Code Quality Foundation

### 1.1 Documentation
- [x] 1.1.1 Create Doxygen configuration file (Doxyfile)
- [x] 1.1.2 Add Doxygen header comments to all check header files
- [x] 1.1.3 Add Sphinx documentation with sphinx-needs traceability
- [x] 1.1.4 Create MISRA rule mapping document
- [x] 1.1.5 Generate and verify Doxygen output

### 1.2 Code Formatting
- [x] 1.2.1 Verify .clang-format configuration matches LLVM style
- [x] 1.2.2 Run clang-format on all source files
- [x] 1.2.3 Add pre-commit hook for clang-format

### 1.3 Linting
- [x] 1.3.1 Configure clang-tidy for self-analysis (.clang-tidy-self)
- [x] 1.3.2 Source code compiles with LLVM strict warning flags (-Wall, -Wextra, -Werror=date-time)
- [x] 1.3.3 Add CI linting step

### 1.4 Security Review
- [x] 1.4.1 Review code for potential security issues (no unsafe functions found)
- [x] 1.4.2 Ensure no hardcoded paths or credentials (verified clean)
- [x] 1.4.3 Validate input handling in all checks (AST-based, no user input)

### 1.5 SonarQube Cloud Setup (Project Quality)
- [x] 1.5.1 Create sonar-project.properties for C++ analysis
- [x] 1.5.2 Configure SonarQube Cloud project for clang-tidy-automotive
- [x] 1.5.3 Add GitHub Actions workflow for SonarQube analysis
- [x] 1.5.4 Configure quality gate thresholds

## 2. Test Infrastructure

### 2.1 Coverage Tooling
- [x] 2.1.1 Configure LLVM build for coverage instrumentation
- [x] 2.1.2 Create coverage collection script
- [x] 2.1.3 Create coverage report generation script
- [x] 2.1.4 Establish baseline coverage measurement

### 2.2 Test Organization
- [x] 2.2.1 Reorganize test directory structure by rule category (created TEST-INDEX.md)
- [x] 2.2.2 Create test naming convention documentation
- [x] 2.2.3 Create test template for new checks

### 2.3 Test Expansion
- [x] 2.3.1 Audit existing tests for completeness
- [x] 2.3.2 Add missing tests for existing checks (bitfield, literal, type, return, stdlib, expression)
- [x] 2.3.3 Add edge case tests for all checks
- [x] 2.3.4 Add negative tests (code that should NOT trigger)

## 3. Examples Directory

### 3.1 Structure
- [x] 3.1.1 Create examples/ directory structure mirroring rule categories
- [x] 3.1.2 Create README.md explaining examples organization
- [x] 3.1.3 Create template for violation/compliant example pairs

### 3.2 Directive Examples
- [x] 3.2.1 Create examples for Dir 1.x (Implementation Environment)
- [x] 3.2.2 Create examples for Dir 2.x (Language Extensions)
- [x] 3.2.3 Create examples for Dir 4.x (Code Design)
- [x] 3.2.4 Create examples for Dir 5.x (Libraries)

### 3.3 Rule Examples (by category)
- [x] 3.3.1 Create examples for Rules 1.x-2.x (Environment, Unused Code)
- [x] 3.3.2 Create examples for Rules 3.x-4.x (Comments, Characters)
- [x] 3.3.3 Create examples for Rules 5.x (Identifiers)
- [x] 3.3.4 Create examples for Rules 6.x-7.x (Types, Literals)
- [x] 3.3.5 Create examples for Rules 8.x-9.x (Declarations, Initialization)
- [x] 3.3.6 Create examples for Rules 10.x-11.x (Conversions, Pointers)
- [x] 3.3.7 Create examples for Rules 12.x-13.x (Expressions)
- [x] 3.3.8 Create examples for Rules 14.x-15.x (Control Flow)
- [x] 3.3.9 Create examples for Rules 16.x (Switch)
- [x] 3.3.10 Create examples for Rules 17.x (Functions)
- [x] 3.3.11 Create examples for Rules 18.x-19.x (Pointers/Arrays, Overlapping)
- [x] 3.3.12 Create examples for Rules 20.x (Preprocessor)
- [x] 3.3.13 Create examples for Rules 21.x-22.x (Standard Library)
- [x] 3.3.14 Create examples for Rules 23.x (Atomics)

## 4. Rule Coverage Expansion

### 4.1 Rule Inventory
- [x] 4.1.1 Create mapping document: MISRA rule -> implemented check (docs/MISRA-RULE-INVENTORY.md, config/misra-rule-mapping.json)
- [x] 4.1.2 Identify rules covered by existing clang-tidy checks
- [x] 4.1.3 Identify rules requiring custom implementation
- [x] 4.1.4 Identify rules requiring manual review only

### 4.2 Check ID Standardization
- [x] 4.2.1 Define new ID naming convention (updated NAMING-CONVENTION.md)
- [x] 4.2.2 Create migration script for ID changes (scripts/check-id-migration.py)
- [x] 4.2.3 Update all check registrations (7 IDs standardized)
- [x] 4.2.4 Update all test file references (5 test files updated)
- [x] 4.2.5 Update documentation (8 doc files updated)

### 4.3 New Check Implementation (Priority 1 - Required Rules)
- [x] 4.3.1 Implement missing Rules 1.x-2.x checks (2.7 implemented, tested)
- [x] 4.3.2 Implement missing Rules 5.x-6.x checks (5.4, 5.6, 5.7 implemented)
- [x] 4.3.3 Implement missing Rules 8.x-9.x checks (8.9 implemented, tested)
- [x] 4.3.4 Implement missing Rules 10.x-11.x checks (10.1, 11.3, 11.4, 11.8 implemented, tested)
- [x] 4.3.5 Implement missing Rules 12.x-13.x checks (12.2, 13.4, 13.5, 13.6 implemented, tested)
- [x] 4.3.6 Implement missing Rules 14.x-17.x checks (14.3, 14.4, 15.4, 17.6 implemented, tested)
- [x] 4.3.7 Implement missing Rules 18.x-19.x checks (18.4 implemented, tested)
- [x] 4.3.8 Implement missing Rules 20.x checks (20.1, 20.2 implemented)
- [x] 4.3.9 Implement missing Rules 21.x-23.x checks (21.1, 21.2 implemented, tested)

### 4.4 New Check Implementation (Priority 2 - Advisory Rules)
- [x] 4.4.1 Implement advisory rules by priority/feasibility
  - Rule 4.2: AvoidTrigraphCheck (automotive-c23-adv-4.2)
  - Rule 12.4: AvoidConstantWrapAroundCheck (automotive-c23-adv-12.4)

### 4.5 Remaining MISRA C:2025 Required Rules
- [x] 4.5.1 Implement Rules 10.2-10.8: Essential type model checks (6 checks)
- [x] 4.5.2 Implement Rules 11.2, 11.5-11.7: Pointer conversion checks (1 combined check)
- [x] 4.5.3 Implement Rules 13.1-13.3: Expression side effect checks (1 combined check)
- [x] 4.5.4 Implement Rules 18.1-18.3, 18.5: Pointer arithmetic checks (1 combined check)
- [x] 4.5.5 Implement Rule 20.3: Include syntax validation (1 check)
- [x] 4.5.6 Implement Rule 9.2: Explicit enumerator values (1 check)
- [x] 4.5.7 Implement Rule 9.4: Incomplete aggregate initialization (1 check)
- [x] 4.5.8 Implement Rule 8.7: Static for internal linkage (1 check)
- [x] 4.5.9 Implement Rule 18.6: Address of automatic object escaping (1 check)
- [x] 4.5.10 Implement Rule 9.5: Designated initializer bounds (1 check)
- [x] 4.5.11 Implement Rule 1.2: Language extensions detection (1 check)
- [x] 4.5.12 Implement Dir 4.8: Pointer hiding in typedefs (1 check)

Note: 95/176 MISRA C:2025 rules implemented (was 94), 25 covered by Clang built-in, 3 remaining planned, 53 require manual review.
Total automotive checks: 104. Registered Rule 21.3 (AvoidStdlibMemoryCheck - dynamic memory allocation).

### 4.6 MISRA C++:2023 Implementation
- [x] 4.6.1 Create MISRA C++:2023 rule inventory document (docs/MISRA-CPP-2023-INVENTORY.md)
- [x] 4.6.2 Identify rules shared with MISRA C:2025 (10 shared checks documented)
- [x] 4.6.3 Identify rules covered by clang-tidy cppcoreguidelines/modernize checks (~75 checks mapped)
- [x] 4.6.4 Implement MISRA C++:2023 specific checks (cpp23 component: 5 checks)
  - AvoidCStyleCastCheck (8.2.1)
  - ExplicitConstructorCheck (15.1.3)
  - AvoidSlicingCheck (15.3)
  - VirtualDestructorCheck (15.7)
  - NoexceptMoveCheck (18.4.2)
- [x] 4.6.5 Add C++ test files for all implemented checks (5 test files)
- [x] 4.6.6 Update documentation for C++ support (README.md, MISRA-CPP-2023-INVENTORY.md)

Note: MISRA C++:2023 is a separate standard with ~200 rules. Many overlap with
existing cppcoreguidelines and modernize checks. Priority should be automotive-specific
rules not covered elsewhere.

### 4.7 Directive Implementation
- [x] 4.7.1 Implement feasible directive checks
  - Rule 15.2: GotoLabelSameBlockCheck (automotive-c23-req-15.2)
  - Dir 4.10: MissingHeaderGuardCheck (automotive-c23-req-dir-4.10)
- [x] 4.7.2 Document manual-review-only directives

## 5. SonarQube Integration

### 5.1 Research
- [x] 5.1.1 Research SonarQube external issue import format
- [x] 5.1.2 Research SARIF format for clang-tidy output
- [x] 5.1.3 Research SonarQube MISRA C plugin capabilities

### 5.2 Implementation
- [x] 5.2.1 Create clang-tidy to SARIF converter (scripts/clang-tidy-to-sarif.py)
- [x] 5.2.2 Create rule mapping file (config/misra-rule-mapping.json)
- [x] 5.2.3 Create SonarQube quality profile for MISRA C:2025 (config/sonarqube-misra-c2025-profile.xml)
- [x] 5.2.4 Create integration test with sample project (scripts/integration-test.sh)

### 5.3 Documentation
- [x] 5.3.1 Document SonarQube integration setup (docs/sonarqube-integration.md)
- [x] 5.3.2 Document compliance reporting workflow
- [x] 5.3.3 Create example CI/CD pipeline configuration

## 6. Final Validation

### 6.1 Coverage Verification
- [x] 6.1.1 Configure CI to generate coverage reports (currently 0% reported)
- [x] 6.1.2 Create coverage build workflow for GitHub Actions (manual trigger)
- [x] 6.1.3 Generate baseline coverage report with llvm-cov
  - Current: 230 files, 2558 lines, 2146 covered (83.89% line coverage)
  - Previous baseline: 79.0% line coverage
- [ ] 6.1.4 Identify untested code paths and add tests
- [ ] 6.1.5 Verify 90%+ code coverage achieved
- [ ] 6.1.6 Document any uncovered code paths with justification
- [x] 6.1.7 Upload coverage report to SonarCloud
  - Using llvm-cov show format (not JSON export)
  - Path rewriting: LLVM tree paths → src/automotive/
- [x] 6.1.8 Document local-only coverage process (docs/CODE-COVERAGE.md)

Note: Coverage testing can ONLY be done locally due to resource constraints.
Requires rebuilding with coverage instrumentation using ./scripts/configure-coverage.sh
followed by full rebuild (~15GB, ~2 hours). Coverage collection is automated via
./scripts/coverage.sh. See docs/CODE-COVERAGE.md for complete instructions.

### 6.2 SonarCloud Code Quality
- [x] 6.2.1 Review 102 maintainability issues reported by SonarCloud
- [x] 6.2.2 Fix high-priority code smells (complexity, duplication)
  - AvoidConstantWrapAroundCheck.cpp: cognitive complexity 62 → ~15
  - AvoidInappropriateEssentialTypeCheck.cpp: cognitive complexity 39 → ~15
  - Extracted helper functions, used early returns
- [x] 6.2.3 Address const_cast issues (4 files with NOSONAR comments)
- [x] 6.2.4 Fix cognitive complexity issues (2 major files refactored)
- [x] 6.2.5 Address unused code warnings (removed 2 unused #include <iostream>)
- [x] 6.2.6 Fix nested if statements (5 files refactored with early returns)
- [x] 6.2.7 Fix remaining naming convention issues
- [x] 6.2.8 Fix remaining cognitive complexity in other files
- [x] 6.2.9 Remove debug output (ImplicitIntCheck.cpp has llvm::outs())
- [ ] 6.2.10 Achieve Quality Gate pass status

Note: SonarCloud dashboard: https://sonarcloud.io/dashboard?id=montge_clang-tidy-automotive
Files fixed in this session:
- AvoidConstantWrapAroundCheck.cpp
- AvoidInappropriateEssentialTypeCheck.cpp/.h
- AvoidSideEffectInLogicalOperandCheck.cpp
- AvoidSideEffectInSizeofCheck.cpp
- UncompleteFunctionPrototypeCheck.cpp
- AvoidOctalNumberCheck.cpp
- MultipleReturnStmtCheck.cpp
- ForwardGotoLabelCheck.cpp
- MultipleLoopTerminatorsCheck.cpp

Additional fixes (current session):
- ImplicitIntCheck.cpp: Removed debug output (llvm::outs())
- ClangTidyDiagnosticMapping.cpp: Removed debug output and unused variable
- AvoidIncompleteInitializationCheck.cpp: Extracted isZeroInitializer() and countExplicitInits() helpers, fixed struct initialization detection
- DuplicateTagNameCheck.cpp: Extracted getTagKindName() helper, simplified duplicate tracking with llvm::find()

Latest fixes:
- AvoidInappropriateCastCheck.cpp: Extracted essentialTypeToString() helper method
- AvoidDesignatedInitializerBoundsCheck.cpp: Extracted checkArrayDesignator() and checkArrayRangeDesignator() helpers
- MissingHeaderGuardCheck.cpp: Refactored FileChanged() and EndOfMainFile() with early returns
- AvoidIncompatiblePointerConversionCheck.cpp: Used early return at end to reduce nesting
- GotoLabelSameBlockCheck.cpp: Simplified getLabelStmt() with early return
- AvoidPointerTypedefCheck.cpp: Extracted checkTypedefDecl() and checkTypeAliasDecl() helpers
- AvoidTrigraphCheck.cpp: Extracted getTrigraphReplacement() helper, used early returns
- VirtualDestructorCheck.cpp: Extracted hasVirtualDestructorInBase() helper
- AvoidSlicingCheck.cpp: Extracted isSlicing() helper function
- AvoidNarrowingCompoundAssignmentCheck.cpp: Extracted essentialTypeToString() helper, replaced nested ternaries
- UniqueEnumValuesCheck.cpp: Early return, simplified ternary
- DuplicateTypedefNameCheck.cpp: Combined early return checks
- AvoidInvalidHeaderCharCheck.cpp: Extracted buildInvalidCharsList() helper
- UnstructuredSwitchStmtCheck.cpp: Removed unnecessary empty else block

### 6.3 Integration Testing
- [x] 6.3.1 Run all checks against examples directory (38/38 violation tests pass)
- [x] 6.3.2 Fix false positives (Rule 9.1 ParmVarDecl fix applied)
- [x] 6.3.3 Verify detection of all known violations (38/38 tests pass)

Note: Compliant examples may trigger unrelated checks since they are designed to
demonstrate compliance with a specific rule, not all rules. This is by design.
True false positive verification requires rule-specific test filtering.

### 6.4 Documentation Review
- [x] 6.4.1 Review all Doxygen documentation for completeness (fixed 15 headers)
- [x] 6.4.2 Update README.md with new capabilities
- [x] 6.4.3 Update CLAUDE.md with new project structure

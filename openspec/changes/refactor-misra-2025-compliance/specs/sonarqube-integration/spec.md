## ADDED Requirements

### Requirement: Project Quality Analysis
The clang-tidy-automotive codebase SHALL be analyzed by SonarQube Cloud for code quality metrics.

#### Scenario: SonarQube Cloud analysis
- **WHEN** code is pushed to the repository
- **THEN** SonarQube Cloud SHALL analyze the C++ codebase

#### Scenario: Quality gate enforcement
- **WHEN** the quality gate fails
- **THEN** the CI pipeline SHALL report the failure

### Requirement: SARIF Output Format
The system SHALL support exporting clang-tidy results in SARIF (Static Analysis Results Interchange Format) format.

#### Scenario: SARIF generation
- **WHEN** clang-tidy is run with SARIF output option
- **THEN** results SHALL be written in valid SARIF 2.1.0 format

#### Scenario: SARIF rule mapping
- **WHEN** a diagnostic is emitted
- **THEN** the SARIF output SHALL include the corresponding MISRA rule ID in the rule metadata

### Requirement: SonarQube Import Compatibility
The system SHALL produce output compatible with SonarQube's generic issue import format.

#### Scenario: Issue import
- **WHEN** SARIF output is imported to SonarQube
- **THEN** issues SHALL appear with correct file locations and severity

#### Scenario: Rule classification
- **WHEN** issues are imported to SonarQube
- **THEN** they SHALL be classified by MISRA rule category (Required, Advisory, Mandatory)

### Requirement: MISRA Compliance Report
The system SHALL enable generation of MISRA compliance reports showing rule coverage.

#### Scenario: Coverage summary
- **WHEN** a compliance report is generated
- **THEN** it SHALL show the number of rules checked vs total rules

#### Scenario: Violation summary
- **WHEN** a compliance report is generated
- **THEN** it SHALL show violations grouped by rule category and severity

### Requirement: Rule Mapping File
The system SHALL maintain a machine-readable mapping file linking automotive checks to MISRA rules.

#### Scenario: Mapping file format
- **WHEN** the mapping file is parsed
- **THEN** it SHALL provide: check ID, MISRA rule ID, coverage level (full/partial/related), and notes

#### Scenario: SonarQube profile generation
- **WHEN** the mapping file is processed
- **THEN** it SHALL enable generation of a SonarQube quality profile for MISRA C:2025

## Known Limitations

### SonarCloud Free Tier Branch Restrictions
- **Current State (2024-2025)**: SonarCloud free tier for open source projects only analyzes the default branch (main/master)
- **Impact**: Feature branches and gitflow workflows are not supported for analysis
- **Workaround**: All code must be merged to main for SonarCloud analysis
- **Future**: SonarCloud plans to introduce branch support for open source projects in 2026

### Recommendation
Until branch support is available, the project should:
1. Keep main branch as the primary development target for SonarCloud analysis
2. Use local clang-tidy runs for branch validation
3. Re-evaluate SonarCloud branch support when available (~2026)

## ADDED Requirements

### Requirement: Code Coverage Measurement
The system SHALL provide tooling to measure code coverage of the automotive checks codebase.

#### Scenario: Coverage report generation
- **WHEN** the coverage script is executed after running tests
- **THEN** a coverage report SHALL be generated showing line and branch coverage percentages

#### Scenario: Coverage threshold enforcement
- **WHEN** code coverage falls below 90%
- **THEN** the coverage tool SHALL report a failure status

### Requirement: Test File Organization
The system SHALL organize test files by MISRA rule category in `test/checkers/automotive/`.

#### Scenario: Test discovery
- **WHEN** tests are executed
- **THEN** all test files in the organized structure SHALL be discovered and run

#### Scenario: Test naming convention
- **WHEN** a new test file is created
- **THEN** it SHALL follow the naming pattern `{rule-id}-{description}.c`

### Requirement: Comprehensive Test Cases
Each automotive check SHALL have test cases covering violation detection and compliant code acceptance.

#### Scenario: Violation detection test
- **WHEN** test code contains a rule violation
- **THEN** the check SHALL report a diagnostic at the expected location

#### Scenario: Compliant code test
- **WHEN** test code is compliant with the rule
- **THEN** the check SHALL NOT report any diagnostic

#### Scenario: Edge case coverage
- **WHEN** test code contains edge cases for the rule
- **THEN** the check SHALL handle them correctly (either detect or accept as appropriate)

### Requirement: Test Template
The system SHALL provide a template for creating new check test files.

#### Scenario: Template usage
- **WHEN** a developer creates a new check
- **THEN** they SHALL be able to copy the template and modify it for the new check

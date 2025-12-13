## ADDED Requirements

### Requirement: Examples Directory Structure
The system SHALL provide an `examples/` directory containing C code examples organized by MISRA rule.

#### Scenario: Directory navigation
- **WHEN** a user browses the examples directory
- **THEN** they SHALL find examples organized by rule category (directives, rules) and rule number

#### Scenario: Example completeness
- **WHEN** an automotive check exists for a MISRA rule
- **THEN** the examples directory SHALL contain corresponding example files

### Requirement: Violation Examples
Each implemented rule SHALL have example code demonstrating violations.

#### Scenario: Violation code structure
- **WHEN** a violation example is examined
- **THEN** it SHALL contain code that triggers the corresponding check
- **THEN** it SHALL include comments indicating the expected diagnostic location

#### Scenario: Multiple violation patterns
- **WHEN** a rule can be violated in multiple ways
- **THEN** the violation example SHALL demonstrate each pattern

### Requirement: Compliant Examples
Each implemented rule SHALL have example code demonstrating compliant usage.

#### Scenario: Compliant code structure
- **WHEN** a compliant example is examined
- **THEN** it SHALL contain code that does NOT trigger the corresponding check
- **THEN** it SHALL demonstrate the correct way to write the code

#### Scenario: Compliant alternatives
- **WHEN** multiple compliant patterns exist
- **THEN** the compliant example SHALL demonstrate recommended alternatives

### Requirement: Example Documentation
Each example directory SHALL contain a README explaining the rule and examples.

#### Scenario: README content
- **WHEN** a user reads the example README
- **THEN** they SHALL understand what the rule checks for (without MISRA copyrighted text)
- **THEN** they SHALL understand why the violation is problematic
- **THEN** they SHALL understand how to fix violations

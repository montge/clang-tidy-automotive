## ADDED Requirements

### Requirement: MISRA C:2025 Rule Coverage
The system SHALL implement clang-tidy checks for MISRA C:2025 rules that are feasible to detect via static analysis.

#### Scenario: Required rule detection
- **WHEN** code violates a Required (req) MISRA C:2025 rule
- **THEN** the corresponding automotive check SHALL report a diagnostic

#### Scenario: Advisory rule detection
- **WHEN** code violates an Advisory (adv) MISRA C:2025 rule
- **THEN** the corresponding automotive check SHALL report a diagnostic

#### Scenario: Compliant code acceptance
- **WHEN** code complies with all MISRA C:2025 rules
- **THEN** no automotive checks SHALL report diagnostics

### Requirement: Standardized Check ID Format
The system SHALL use check IDs following the pattern `automotive-c25-{category}-{rule.subrule}` where category is `req`, `adv`, `man`, or `dir`.

#### Scenario: Check ID format validation
- **WHEN** a check is registered
- **THEN** its ID SHALL match the pattern `automotive-c25-(req|adv|man|dir)-\d+\.\d+`

#### Scenario: Legacy ID deprecation
- **WHEN** a legacy check ID is used in configuration
- **THEN** the system SHALL emit a deprecation warning with the new ID

### Requirement: Rule Mapping Documentation
The system SHALL maintain a mapping document linking each automotive check to its corresponding MISRA C:2025 rule(s).

#### Scenario: Mapping completeness
- **WHEN** the mapping document is reviewed
- **THEN** every automotive check SHALL be mapped to at least one MISRA rule

#### Scenario: Unmapped rule documentation
- **WHEN** a MISRA rule cannot be implemented as a check
- **THEN** the mapping document SHALL document the reason and suggest manual review procedures

### Requirement: Multi-Version LLVM Compatibility
The system SHALL compile and function correctly with LLVM/Clang versions 17.x, 18.x, 19.x, and 20.x.

#### Scenario: LLVM 17 compilation
- **WHEN** building with LLVM 17.x
- **THEN** all checks SHALL compile without errors

#### Scenario: LLVM 20 feature usage
- **WHEN** a newer LLVM API is available
- **THEN** conditional compilation SHALL enable the newer API while maintaining backward compatibility

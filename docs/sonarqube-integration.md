# SonarQube Integration Guide

This guide explains how to integrate clang-tidy-automotive with SonarQube (on-premise) and SonarCloud for MISRA C:2025 compliance reporting.

## Overview

clang-tidy-automotive can report findings to SonarQube through two methods:

1. **Generic Issue Import** - Works with all SonarQube versions (recommended for on-premise)
2. **SARIF Import** - Requires SonarQube 9.6+ or specific plugins

## Quick Start

### For Open Source Projects (SonarCloud)

1. Sign up at [sonarcloud.io](https://sonarcloud.io) with your GitHub account
2. Import your repository
3. Add the `SONAR_TOKEN` secret to your GitHub repository
4. Use the included GitHub Actions workflow (`.github/workflows/sonarcloud.yml`)

### For On-Premise SonarQube

1. Install SonarQube (Community Edition or higher)
2. Configure the project in SonarQube
3. Run clang-tidy and convert output to SonarQube format
4. Import issues using sonar-scanner

## Detailed Setup

### Prerequisites

- Python 3.8+ (for conversion scripts)
- clang-tidy with automotive checks built
- SonarQube 8.9+ or SonarCloud account

### Method 1: Generic Issue Import (Recommended)

This method works with any SonarQube version and doesn't require additional plugins.

#### Step 1: Run clang-tidy analysis

```bash
# Run clang-tidy on your codebase
./build/bin/clang-tidy \
    --checks="-*,automotive-*" \
    -p build \
    src/**/*.c > clang-tidy-output.txt 2>&1
```

#### Step 2: Convert to SonarQube format

```bash
# Convert clang-tidy output to SonarQube Generic Issue format
python3 scripts/clang-tidy-to-sonarqube.py \
    --input clang-tidy-output.txt \
    --output sonarqube-issues.json \
    --base-path $(pwd) \
    --pretty
```

#### Step 3: Configure sonar-project.properties

```properties
# Project identification
sonar.projectKey=your-project-key
sonar.projectName=Your Project Name
sonar.organization=your-org  # For SonarCloud only

# Source code
sonar.sources=src
sonar.sourceEncoding=UTF-8

# External issues from clang-tidy-automotive
sonar.externalIssuesReportPaths=sonarqube-issues.json

# Coverage (optional)
sonar.cfamily.llvm-cov.reportPath=coverage.lcov
```

#### Step 4: Run sonar-scanner

```bash
sonar-scanner \
    -Dsonar.host.url=https://your-sonarqube-server \
    -Dsonar.token=$SONAR_TOKEN
```

### Method 2: SARIF Import

For organizations using GitHub Advanced Security or SonarQube with SARIF support.

#### Step 1: Generate SARIF output

```bash
./build/bin/clang-tidy \
    --checks="-*,automotive-*" \
    -p build \
    src/**/*.c 2>&1 | \
python3 scripts/clang-tidy-to-sarif.py \
    --output results.sarif \
    --pretty
```

#### Step 2: Import to SonarQube

For SonarQube 9.6+:
```bash
sonar-scanner \
    -Dsonar.sarifReportPaths=results.sarif
```

For GitHub Code Scanning:
```bash
# Upload SARIF to GitHub Security tab
gh api \
    -X POST \
    -H "Accept: application/vnd.github+json" \
    /repos/OWNER/REPO/code-scanning/sarifs \
    -f commit_sha=$(git rev-parse HEAD) \
    -f ref=refs/heads/main \
    -f sarif=@results.sarif
```

## GitHub Actions Integration

### SonarCloud Workflow

The repository includes `.github/workflows/sonarcloud.yml` which:

1. Builds clang-tidy with coverage instrumentation
2. Runs tests to collect coverage data
3. Generates LCOV coverage report
4. Uploads to SonarCloud with build-wrapper

#### Required Secrets

Add these secrets to your GitHub repository:

| Secret | Description |
|--------|-------------|
| `SONAR_TOKEN` | SonarCloud authentication token |

#### Getting a SonarCloud Token

1. Log in to [sonarcloud.io](https://sonarcloud.io)
2. Go to My Account > Security
3. Generate a new token
4. Add it as `SONAR_TOKEN` in GitHub repository settings

### On-Premise Workflow Example

```yaml
name: SonarQube Analysis

on:
  push:
    branches: [main]
  pull_request:
    types: [opened, synchronize, reopened]

jobs:
  sonarqube:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v4
        with:
          fetch-depth: 0

      - name: Build clang-tidy-automotive
        run: |
          ./download.sh
          ./setup.sh
          ./configure.sh
          ./build.sh

      - name: Run analysis
        run: |
          find src -name "*.c" -exec \
            ./build/bin/clang-tidy {} \
            --checks="-*,automotive-*" \
            -- -std=c11 \; > clang-tidy-output.txt 2>&1 || true

      - name: Convert to SonarQube format
        run: |
          python3 scripts/clang-tidy-to-sonarqube.py \
            --input clang-tidy-output.txt \
            --output sonarqube-issues.json \
            --base-path $(pwd)

      - name: SonarQube Scan
        uses: SonarSource/sonarqube-scan-action@master
        env:
          SONAR_TOKEN: ${{ secrets.SONAR_TOKEN }}
          SONAR_HOST_URL: ${{ secrets.SONAR_HOST_URL }}
```

## Coverage Reporting

### Generating Coverage Data

```bash
# Configure build with coverage
mkdir -p build && cd build
cmake -G Ninja \
    -DCMAKE_CXX_FLAGS="-fprofile-instr-generate -fcoverage-mapping" \
    -DCMAKE_C_FLAGS="-fprofile-instr-generate -fcoverage-mapping" \
    ../llvm-project-llvmorg-20.1.8/llvm

# Build
ninja clang-tidy

# Run tests with coverage
export LLVM_PROFILE_FILE="coverage-%p.profraw"
./bin/clang-tidy ../test/checkers/automotive/**/*.c --checks="automotive-*" -- || true

# Merge and convert
llvm-profdata merge -sparse coverage-*.profraw -o coverage.profdata
llvm-cov export ./bin/clang-tidy \
    -instr-profile=coverage.profdata \
    -format=lcov \
    -ignore-filename-regex='llvm-project-.*' \
    > coverage.lcov
```

### SonarQube Coverage Configuration

```properties
# In sonar-project.properties
sonar.cfamily.llvm-cov.reportPath=coverage.lcov
```

## Quality Gate Configuration

### Recommended Quality Gate Thresholds

For safety-critical automotive software, configure your quality gate with these thresholds:

| Metric | Threshold | Rationale |
|--------|-----------|-----------|
| Coverage on new code | >= 80% | High coverage for safety-critical code |
| Duplicated lines on new code | <= 3% | Minimize code duplication |
| Maintainability rating | A | Clean, maintainable code |
| Reliability rating | A | No bugs introduced |
| Security rating | A | No vulnerabilities |
| Security hotspots reviewed | 100% | All security concerns addressed |

### Configuring in SonarCloud

1. Go to your project in SonarCloud
2. Navigate to **Project Settings** > **Quality Gates**
3. Either use "Sonar way" (default) or create a custom gate
4. For custom gate, click **Create** and set:
   - Condition: Coverage on New Code >= 80
   - Condition: Duplicated Lines (%) on New Code <= 3
   - Condition: Maintainability Rating on New Code is A
   - Condition: Reliability Rating on New Code is A
   - Condition: Security Rating on New Code is A

### Configuring in On-Premise SonarQube

```bash
# Create quality gate via API
curl -u admin:admin -X POST \
  "http://localhost:9000/api/qualitygates/create?name=Automotive%20Safety"

# Add conditions
curl -u admin:admin -X POST \
  "http://localhost:9000/api/qualitygates/create_condition" \
  -d "gateId=1&metric=new_coverage&op=LT&error=80"
```

### Quality Gate in CI/CD

The `sonar.qualitygate.wait=true` setting in `sonar-project.properties` ensures:
- CI pipeline waits for SonarCloud analysis to complete
- Pipeline fails if quality gate fails
- Prevents merging code that doesn't meet quality standards

## MISRA Compliance Dashboard

### Custom Quality Profile

Create a SonarQube quality profile for MISRA C:2025:

1. Go to Quality Profiles in SonarQube
2. Create a new profile based on "Sonar way"
3. Add rules from clang-tidy-automotive via external issues
4. Set appropriate severities based on MISRA categories

### Rule Severity Mapping

| MISRA Category | SonarQube Severity |
|----------------|-------------------|
| Mandatory | BLOCKER |
| Required | CRITICAL |
| Advisory | MAJOR |

### Compliance Reporting

Generate a compliance report showing MISRA rule coverage:

```bash
# Generate compliance summary
python3 scripts/misra-compliance-report.py \
    --mapping config/misra-rule-mapping.json \
    --issues sonarqube-issues.json \
    --output compliance-report.html
```

## Troubleshooting

### Common Issues

#### "No issues imported"

- Verify the JSON file is valid: `python3 -m json.tool sonarqube-issues.json`
- Check file paths are relative to project root
- Ensure `sonar.externalIssuesReportPaths` is set correctly

#### "Coverage not showing"

- Verify LCOV format is correct
- Check that file paths in LCOV match source paths
- Ensure coverage data was generated with instrumented build

#### "Analysis timeout"

- Increase scanner timeout: `-Dsonar.scanner.socketTimeout=600`
- Use incremental analysis for large codebases
- Consider running analysis on changed files only

### Debug Mode

Run sonar-scanner with debug output:

```bash
sonar-scanner -X \
    -Dsonar.verbose=true \
    -Dsonar.log.level=DEBUG
```

## Integration with CI/CD

### Jenkins Pipeline

```groovy
pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                sh './build.sh'
            }
        }

        stage('Analyze') {
            steps {
                sh '''
                    ./build/bin/clang-tidy \
                        --checks="-*,automotive-*" \
                        -p build \
                        src/**/*.c > clang-tidy-output.txt 2>&1 || true

                    python3 scripts/clang-tidy-to-sonarqube.py \
                        --input clang-tidy-output.txt \
                        --output sonarqube-issues.json
                '''
            }
        }

        stage('SonarQube') {
            steps {
                withSonarQubeEnv('SonarQube') {
                    sh 'sonar-scanner'
                }
            }
        }

        stage('Quality Gate') {
            steps {
                timeout(time: 1, unit: 'HOURS') {
                    waitForQualityGate abortPipeline: true
                }
            }
        }
    }
}
```

### GitLab CI

```yaml
sonarqube:
  stage: analyze
  image: sonarsource/sonar-scanner-cli
  variables:
    SONAR_USER_HOME: "${CI_PROJECT_DIR}/.sonar"
  cache:
    key: "${CI_JOB_NAME}"
    paths:
      - .sonar/cache
  script:
    - ./build.sh
    - ./build/bin/clang-tidy --checks="-*,automotive-*" -p build src/**/*.c > output.txt 2>&1 || true
    - python3 scripts/clang-tidy-to-sonarqube.py -i output.txt -o issues.json
    - sonar-scanner
  only:
    - merge_requests
    - main
```

## References

- [SonarQube Generic Issue Data](https://docs.sonarqube.org/latest/analysis/generic-issue/)
- [SARIF Specification](https://docs.oasis-open.org/sarif/sarif/v2.1.0/sarif-v2.1.0.html)
- [SonarCloud Documentation](https://docs.sonarcloud.io/)
- [MISRA C:2025 Guidelines](https://www.misra.org.uk/)

#!/bin/bash
# Integration Test Script for clang-tidy-automotive
#
# Tests the automotive checks against the examples directory
# and generates a compliance report.
#
# Usage:
#   ./scripts/integration-test.sh [--verbose] [--report]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${PROJECT_ROOT}/build"
EXAMPLES_DIR="${PROJECT_ROOT}/examples"
OUTPUT_DIR="${PROJECT_ROOT}/test-results"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

VERBOSE=false
GENERATE_REPORT=false

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --verbose|-v)
            VERBOSE=true
            shift
            ;;
        --report|-r)
            GENERATE_REPORT=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--verbose] [--report]"
            exit 1
            ;;
    esac
done

CLANG_TIDY="${BUILD_DIR}/bin/clang-tidy"

# Check prerequisites
check_prerequisites() {
    echo -e "${YELLOW}Checking prerequisites...${NC}"

    if [ ! -f "$CLANG_TIDY" ]; then
        echo -e "${RED}Error: clang-tidy not found at $CLANG_TIDY${NC}"
        echo "Build the project first with: ./build.sh"
        exit 1
    fi

    if [ ! -d "$EXAMPLES_DIR" ]; then
        echo -e "${RED}Error: Examples directory not found at $EXAMPLES_DIR${NC}"
        exit 1
    fi

    mkdir -p "$OUTPUT_DIR"
    echo -e "${GREEN}Prerequisites OK${NC}"
}

# Test violation examples (should produce diagnostics)
test_violations() {
    echo ""
    echo -e "${YELLOW}Testing violation examples...${NC}"

    local passed=0
    local failed=0
    local total=0

    while IFS= read -r -d '' file; do
        total=$((total + 1))
        filename=$(basename "$file")

        if $VERBOSE; then
            echo -n "  Testing: $filename ... "
        fi

        # Run clang-tidy and capture output
        output=$("$CLANG_TIDY" "$file" --checks="-*,automotive-*" -- -std=c11 2>&1) || true

        # Check if any automotive warning was produced
        if echo "$output" | grep -q '\[automotive-'; then
            passed=$((passed + 1))
            if $VERBOSE; then
                echo -e "${GREEN}PASS${NC}"
            fi
        else
            failed=$((failed + 1))
            if $VERBOSE; then
                echo -e "${RED}FAIL (no diagnostic)${NC}"
            else
                echo -e "${RED}FAIL: $filename - expected diagnostic not found${NC}"
            fi
        fi
    done < <(find "$EXAMPLES_DIR" -name "*-violation.c" -print0)

    echo ""
    echo "Violation tests: $passed/$total passed"

    if [ $failed -gt 0 ]; then
        return 1
    fi
    return 0
}

# Test compliant examples (should NOT produce diagnostics)
test_compliant() {
    echo ""
    echo -e "${YELLOW}Testing compliant examples...${NC}"

    local passed=0
    local failed=0
    local total=0

    while IFS= read -r -d '' file; do
        total=$((total + 1))
        filename=$(basename "$file")

        if $VERBOSE; then
            echo -n "  Testing: $filename ... "
        fi

        # Run clang-tidy and capture output
        output=$("$CLANG_TIDY" "$file" --checks="-*,automotive-*" -- -std=c11 2>&1) || true

        # Check if any automotive warning was produced
        if echo "$output" | grep -q '\[automotive-'; then
            failed=$((failed + 1))
            if $VERBOSE; then
                echo -e "${RED}FAIL (unexpected diagnostic)${NC}"
            else
                echo -e "${RED}FAIL: $filename - unexpected diagnostic found${NC}"
            fi
        else
            passed=$((passed + 1))
            if $VERBOSE; then
                echo -e "${GREEN}PASS${NC}"
            fi
        fi
    done < <(find "$EXAMPLES_DIR" -name "*-compliant.c" -print0)

    echo ""
    echo "Compliant tests: $passed/$total passed"

    if [ $failed -gt 0 ]; then
        return 1
    fi
    return 0
}

# Generate full analysis report
generate_report() {
    echo ""
    echo -e "${YELLOW}Generating analysis report...${NC}"

    # Run clang-tidy on all examples
    "$CLANG_TIDY" $(find "$EXAMPLES_DIR" -name "*.c") \
        --checks="-*,automotive-*" \
        -- -std=c11 2>&1 > "$OUTPUT_DIR/clang-tidy-output.txt" || true

    # Convert to SonarQube format
    if [ -f "$SCRIPT_DIR/clang-tidy-to-sonarqube.py" ]; then
        python3 "$SCRIPT_DIR/clang-tidy-to-sonarqube.py" \
            --input "$OUTPUT_DIR/clang-tidy-output.txt" \
            --output "$OUTPUT_DIR/sonarqube-issues.json" \
            --base-path "$PROJECT_ROOT" \
            --pretty
        echo "  Generated: $OUTPUT_DIR/sonarqube-issues.json"
    fi

    # Convert to SARIF format
    if [ -f "$SCRIPT_DIR/clang-tidy-to-sarif.py" ]; then
        python3 "$SCRIPT_DIR/clang-tidy-to-sarif.py" \
            --input "$OUTPUT_DIR/clang-tidy-output.txt" \
            --output "$OUTPUT_DIR/results.sarif" \
            --pretty
        echo "  Generated: $OUTPUT_DIR/results.sarif"
    fi

    # Generate compliance report
    if [ -f "$SCRIPT_DIR/misra-compliance-report.py" ]; then
        python3 "$SCRIPT_DIR/misra-compliance-report.py" \
            --mapping "$PROJECT_ROOT/config/misra-rule-mapping.json" \
            --issues "$OUTPUT_DIR/sonarqube-issues.json" \
            --output "$OUTPUT_DIR/compliance-report.html"
        echo "  Generated: $OUTPUT_DIR/compliance-report.html"
    fi

    echo -e "${GREEN}Report generation complete${NC}"
}

# Main
main() {
    echo "================================"
    echo " Integration Test Suite"
    echo "================================"

    check_prerequisites

    violation_result=0
    compliant_result=0

    test_violations || violation_result=$?
    test_compliant || compliant_result=$?

    if $GENERATE_REPORT; then
        generate_report
    fi

    echo ""
    echo "================================"
    if [ $violation_result -eq 0 ] && [ $compliant_result -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed${NC}"
        exit 1
    fi
}

main

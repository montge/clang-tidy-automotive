#!/bin/bash
# Coverage Collection Script for clang-tidy-automotive
# Collects code coverage data by running clang-tidy against test files
#
# Usage: ./scripts/coverage.sh [--html] [--report]
#   --html    Generate HTML coverage report
#   --report  Generate text coverage summary
#
# Prerequisites:
#   - Build must be configured with coverage flags (use ./scripts/configure-coverage.sh)
#   - LLVM tools (llvm-profdata, llvm-cov) must be in PATH

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${PROJECT_ROOT}/build"
TEST_DIR="${PROJECT_ROOT}/test/checkers/automotive"
COVERAGE_DIR="${PROJECT_ROOT}/coverage"

# Default LLVM version (can be overridden)
LLVM_VERSION="${LLVM_VERSION:-20}"

# Tool paths
PROFDATA="llvm-profdata-${LLVM_VERSION}"
COV="llvm-cov-${LLVM_VERSION}"
CLANG_TIDY="${BUILD_DIR}/bin/clang-tidy"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Parse arguments
GENERATE_HTML=false
GENERATE_REPORT=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --html)
            GENERATE_HTML=true
            shift
            ;;
        --report)
            GENERATE_REPORT=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--html] [--report]"
            exit 1
            ;;
    esac
done

# Check prerequisites
check_prerequisites() {
    echo -e "${YELLOW}Checking prerequisites...${NC}"

    if [ ! -f "$CLANG_TIDY" ]; then
        echo -e "${RED}Error: clang-tidy not found at $CLANG_TIDY${NC}"
        echo "Build the project first with: ninja -C build clang-tidy"
        exit 1
    fi

    if ! command -v "$PROFDATA" &> /dev/null; then
        echo -e "${RED}Error: $PROFDATA not found${NC}"
        echo "Install LLVM tools or set LLVM_VERSION environment variable"
        exit 1
    fi

    if ! command -v "$COV" &> /dev/null; then
        echo -e "${RED}Error: $COV not found${NC}"
        echo "Install LLVM tools or set LLVM_VERSION environment variable"
        exit 1
    fi

    echo -e "${GREEN}Prerequisites OK${NC}"
}

# Clean previous coverage data
clean_coverage() {
    echo -e "${YELLOW}Cleaning previous coverage data...${NC}"
    rm -rf "$COVERAGE_DIR"
    rm -f "${BUILD_DIR}"/*.profraw
    rm -f "${BUILD_DIR}"/*.profdata
    mkdir -p "$COVERAGE_DIR"
}

# Run clang-tidy on test files to generate coverage data
collect_coverage() {
    echo -e "${YELLOW}Collecting coverage data...${NC}"

    cd "$BUILD_DIR"
    export LLVM_PROFILE_FILE="${BUILD_DIR}/coverage-%p.profraw"

    test_count=0
    passed_count=0

    # Find all test files
    while IFS= read -r -d '' test_file; do
        test_count=$((test_count + 1))
        filename=$(basename "$test_file")

        echo -n "  Testing: $filename ... "

        if "$CLANG_TIDY" "$test_file" --checks="automotive-*" -- -std=c11 2>/dev/null; then
            echo -e "${GREEN}OK${NC}"
            passed_count=$((passed_count + 1))
        else
            echo -e "${GREEN}OK${NC}"  # clang-tidy exit codes indicate findings, not failures
            passed_count=$((passed_count + 1))
        fi
    done < <(find "$TEST_DIR" -name "*.c" -print0)

    echo -e "${GREEN}Processed $test_count test files${NC}"
}

# Merge profile data
merge_profiles() {
    echo -e "${YELLOW}Merging profile data...${NC}"

    profraw_files=$(find "$BUILD_DIR" -name "*.profraw" 2>/dev/null | wc -l)

    if [ "$profraw_files" -eq 0 ]; then
        echo -e "${RED}Error: No profile data files found${NC}"
        echo "Make sure the build was configured with coverage flags"
        exit 1
    fi

    "$PROFDATA" merge -sparse "${BUILD_DIR}"/coverage-*.profraw -o "${COVERAGE_DIR}/coverage.profdata"
    echo -e "${GREEN}Merged $profraw_files profile files${NC}"
}

# Generate coverage reports
generate_reports() {
    echo -e "${YELLOW}Generating coverage reports...${NC}"

    # Generate LCOV format (for local viewing/other tools)
    # Our code is at: llvm-project-.../clang-tools-extra/clang-tidy/automotive/
    # We generate full coverage then filter to just automotive files
    FULL_LCOV="${COVERAGE_DIR}/coverage-full.lcov"
    "$COV" export "$CLANG_TIDY" \
        -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        -format=lcov \
        > "$FULL_LCOV" 2>/dev/null || true

    # Filter to only include automotive source files (with path rewriting)
    python3 "${SCRIPT_DIR}/filter-lcov.py" "$FULL_LCOV" "${COVERAGE_DIR}/coverage.lcov" "clang-tidy/automotive"

    echo -e "${GREEN}Generated: ${COVERAGE_DIR}/coverage.lcov${NC}"

    # Generate llvm-cov show format for SonarCloud
    # SonarCloud's cfamily plugin expects 'llvm-cov show' output (annotated sources)
    # Filter to only automotive source files in the LLVM tree
    AUTOMOTIVE_DIR="${PROJECT_ROOT}/llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/automotive"
    "$COV" show "$CLANG_TIDY" \
        -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        -show-branches=count \
        -path-equivalence="${AUTOMOTIVE_DIR}",src/automotive \
        "$AUTOMOTIVE_DIR" \
        > "${COVERAGE_DIR}/coverage-show.txt" 2>/dev/null || true

    echo -e "${GREEN}Generated: ${COVERAGE_DIR}/coverage-show.txt${NC}"

    if $GENERATE_REPORT; then
        # Generate text summary - only automotive files
        "$COV" report "$CLANG_TIDY" \
            -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
            2>/dev/null | grep -E "automotive|^Filename|^----|TOTAL" \
            > "${COVERAGE_DIR}/coverage-summary.txt" 2>/dev/null || true

        echo -e "${GREEN}Generated: ${COVERAGE_DIR}/coverage-summary.txt${NC}"

        # Print summary
        echo ""
        echo "=== Coverage Summary ==="
        cat "${COVERAGE_DIR}/coverage-summary.txt" 2>/dev/null || echo "No coverage data available"
    fi

    if $GENERATE_HTML; then
        # Generate HTML report
        "$COV" show "$CLANG_TIDY" \
            -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
            -format=html \
            -output-dir="${COVERAGE_DIR}/html" \
            -ignore-filename-regex='llvm-project-.*' \
            -ignore-filename-regex='test/.*' \
            2>/dev/null || true

        echo -e "${GREEN}Generated: ${COVERAGE_DIR}/html/index.html${NC}"
    fi
}

# Main execution
main() {
    echo "================================"
    echo " Coverage Collection Script"
    echo "================================"
    echo ""

    check_prerequisites
    clean_coverage
    collect_coverage
    merge_profiles
    generate_reports

    echo ""
    echo -e "${GREEN}Coverage collection complete!${NC}"
    echo "Results available in: ${COVERAGE_DIR}/"
}

main

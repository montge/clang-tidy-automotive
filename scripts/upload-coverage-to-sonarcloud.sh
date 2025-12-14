#!/bin/bash
# Upload Local Coverage to SonarCloud
# This script uploads locally-generated coverage data to SonarCloud.
#
# Usage: ./scripts/upload-coverage-to-sonarcloud.sh [coverage.lcov]
#
# Prerequisites:
#   - sonar-scanner installed: npm install -g sonar-scanner or brew install sonar-scanner
#   - SONAR_TOKEN environment variable set
#   - Coverage file generated locally (./scripts/coverage.sh)
#
# Why local coverage?
#   Full LLVM builds with coverage instrumentation require ~15GB disk space
#   and ~2 hours build time. GitHub Actions runners run out of storage.
#   Generate coverage locally and upload with this script.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
COVERAGE_FILE="${1:-${PROJECT_ROOT}/coverage/coverage.lcov}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "================================"
echo " SonarCloud Coverage Upload"
echo "================================"
echo ""

# Check prerequisites
if [ -z "$SONAR_TOKEN" ]; then
    echo -e "${RED}Error: SONAR_TOKEN environment variable not set${NC}"
    echo ""
    echo "Get your token from: https://sonarcloud.io/account/security"
    echo "Then run: export SONAR_TOKEN=your_token"
    exit 1
fi

if ! command -v sonar-scanner &> /dev/null; then
    echo -e "${RED}Error: sonar-scanner not found${NC}"
    echo ""
    echo "Install with one of:"
    echo "  npm install -g sonar-scanner"
    echo "  brew install sonar-scanner"
    exit 1
fi

if [ ! -f "$COVERAGE_FILE" ]; then
    echo -e "${RED}Error: Coverage file not found: $COVERAGE_FILE${NC}"
    echo ""
    echo "Generate coverage first:"
    echo "  1. Configure build with coverage: ./scripts/configure-coverage.sh"
    echo "  2. Rebuild: ./build.sh"
    echo "  3. Collect coverage: ./scripts/coverage.sh"
    exit 1
fi

echo -e "${GREEN}Found coverage file: $COVERAGE_FILE${NC}"
echo ""

# Check coverage file has content
lines=$(wc -l < "$COVERAGE_FILE" 2>/dev/null || echo "0")
if [ "$lines" -lt 10 ]; then
    echo -e "${YELLOW}Warning: Coverage file appears to be small or empty ($lines lines)${NC}"
fi

echo -e "${YELLOW}Uploading coverage to SonarCloud...${NC}"
echo ""

cd "$PROJECT_ROOT"

# Run sonar-scanner with coverage
sonar-scanner \
    -Dsonar.projectKey=montge_clang-tidy-automotive \
    -Dsonar.organization=montge \
    -Dsonar.sources=src/automotive \
    -Dsonar.tests=test/checkers/automotive \
    -Dsonar.sourceEncoding=UTF-8 \
    -Dsonar.cfamily.llvm-cov.reportPath="$COVERAGE_FILE" \
    -Dsonar.host.url=https://sonarcloud.io \
    -Dsonar.token="$SONAR_TOKEN"

echo ""
echo -e "${GREEN}Coverage upload complete!${NC}"
echo ""
echo "View results at: https://sonarcloud.io/dashboard?id=montge_clang-tidy-automotive"

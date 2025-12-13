#!/bin/bash
# Build documentation for production
#
# Usage: ./build.sh [output_dir]
#   output_dir: Optional output directory (default: _site)
#
# This script builds the Jekyll documentation for deployment.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT_DIR="${1:-_site}"

cd "$SCRIPT_DIR"

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}Building documentation...${NC}"

# Check dependencies
if ! command -v bundle &> /dev/null; then
    echo "Installing Bundler..."
    gem install bundler
fi

# Install dependencies
bundle install --quiet

# Clean previous build
rm -rf "$OUTPUT_DIR"

# Build for production
JEKYLL_ENV=production bundle exec jekyll build \
    --destination "$OUTPUT_DIR"

# Report
echo ""
echo -e "${GREEN}Build complete!${NC}"
echo "Output: ${SCRIPT_DIR}/${OUTPUT_DIR}"
echo ""

# File count
file_count=$(find "$OUTPUT_DIR" -type f | wc -l)
echo "Generated $file_count files"

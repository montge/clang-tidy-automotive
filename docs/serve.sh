#!/bin/bash
# Local development server for documentation
#
# Usage: ./serve.sh [port]
#   port: Optional port number (default: 4000)
#
# Prerequisites:
#   - Ruby 3.0+
#   - Bundler gem

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PORT="${1:-4000}"

cd "$SCRIPT_DIR"

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}Setting up Jekyll documentation server...${NC}"

# Check Ruby
if ! command -v ruby &> /dev/null; then
    echo "Error: Ruby is not installed"
    echo "Install with: sudo apt install ruby ruby-dev"
    exit 1
fi

# Check Bundler
if ! command -v bundle &> /dev/null; then
    echo "Installing Bundler..."
    gem install bundler
fi

# Install dependencies
if [ ! -f "Gemfile.lock" ] || [ "Gemfile" -nt "Gemfile.lock" ]; then
    echo -e "${YELLOW}Installing Jekyll dependencies...${NC}"
    bundle install
fi

echo ""
echo -e "${GREEN}Starting Jekyll server on http://localhost:${PORT}${NC}"
echo "Press Ctrl+C to stop"
echo ""

# Serve with live reload
bundle exec jekyll serve \
    --host 0.0.0.0 \
    --port "$PORT" \
    --livereload \
    --incremental

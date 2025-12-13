#!/bin/bash
# Run Jekyll documentation server in Docker
#
# Usage: ./docker-serve.sh [command]
#   (no args)  - Start development server with live reload
#   build      - Build static site for production
#   stop       - Stop running container
#   logs       - Show container logs
#   shell      - Open shell in container
#
# Requirements:
#   - Docker or Podman
#   - docker-compose (optional, for compose mode)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Detect container runtime
if command -v docker &> /dev/null; then
    RUNTIME="docker"
    COMPOSE="docker-compose"
    if docker compose version &> /dev/null 2>&1; then
        COMPOSE="docker compose"
    fi
elif command -v podman &> /dev/null; then
    RUNTIME="podman"
    COMPOSE="podman-compose"
else
    echo -e "${RED}Error: Docker or Podman not found${NC}"
    echo "Install Docker: https://docs.docker.com/get-docker/"
    exit 1
fi

IMAGE_NAME="clang-tidy-docs"
CONTAINER_NAME="clang-tidy-docs"

case "${1:-serve}" in
    serve|start|"")
        echo -e "${YELLOW}Starting Jekyll server in Docker...${NC}"

        # Use docker-compose if available
        if [ -f "docker-compose.yml" ] && command -v $COMPOSE &> /dev/null; then
            $COMPOSE up --build
        else
            # Fallback to direct docker run
            $RUNTIME build -t "$IMAGE_NAME" .
            $RUNTIME run --rm -it \
                -p 4000:4000 \
                -p 35729:35729 \
                -v "$(pwd):/srv/jekyll:cached" \
                --name "$CONTAINER_NAME" \
                "$IMAGE_NAME"
        fi
        ;;

    build)
        echo -e "${YELLOW}Building static site...${NC}"

        if [ -f "docker-compose.yml" ] && command -v $COMPOSE &> /dev/null; then
            $COMPOSE run --rm build
        else
            $RUNTIME build -t "$IMAGE_NAME" .
            $RUNTIME run --rm \
                -v "$(pwd):/srv/jekyll:cached" \
                -e JEKYLL_ENV=production \
                "$IMAGE_NAME" \
                bundle exec jekyll build --destination /srv/jekyll/_site
        fi

        echo -e "${GREEN}Build complete!${NC}"
        echo "Output: ${SCRIPT_DIR}/_site"
        ;;

    stop)
        echo -e "${YELLOW}Stopping container...${NC}"
        if [ -f "docker-compose.yml" ] && command -v $COMPOSE &> /dev/null; then
            $COMPOSE down
        else
            $RUNTIME stop "$CONTAINER_NAME" 2>/dev/null || true
        fi
        echo -e "${GREEN}Stopped${NC}"
        ;;

    logs)
        if [ -f "docker-compose.yml" ] && command -v $COMPOSE &> /dev/null; then
            $COMPOSE logs -f
        else
            $RUNTIME logs -f "$CONTAINER_NAME"
        fi
        ;;

    shell)
        echo -e "${YELLOW}Opening shell in container...${NC}"
        $RUNTIME exec -it "$CONTAINER_NAME" /bin/bash
        ;;

    clean)
        echo -e "${YELLOW}Cleaning up...${NC}"
        if [ -f "docker-compose.yml" ] && command -v $COMPOSE &> /dev/null; then
            $COMPOSE down -v --rmi local
        else
            $RUNTIME stop "$CONTAINER_NAME" 2>/dev/null || true
            $RUNTIME rm "$CONTAINER_NAME" 2>/dev/null || true
            $RUNTIME rmi "$IMAGE_NAME" 2>/dev/null || true
        fi
        rm -rf _site .jekyll-cache
        echo -e "${GREEN}Cleaned${NC}"
        ;;

    *)
        echo "Usage: $0 [serve|build|stop|logs|shell|clean]"
        echo ""
        echo "Commands:"
        echo "  serve  - Start development server (default)"
        echo "  build  - Build static site for production"
        echo "  stop   - Stop running container"
        echo "  logs   - Show container logs"
        echo "  shell  - Open shell in container"
        echo "  clean  - Remove containers, images, and generated files"
        exit 1
        ;;
esac

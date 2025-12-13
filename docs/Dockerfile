# Jekyll Documentation Server
# Build and serve the clang-tidy-automotive documentation
#
# Usage:
#   docker build -t clang-tidy-docs .
#   docker run -p 4000:4000 -v $(pwd):/srv/jekyll clang-tidy-docs

FROM ruby:3.2-slim

# Install build dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /srv/jekyll

# Copy Gemfile first for layer caching
COPY Gemfile Gemfile.lock* ./

# Install Jekyll and dependencies
RUN bundle install --jobs 4

# Copy documentation files
COPY . .

# Expose Jekyll default port
EXPOSE 4000

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=60s --retries=3 \
    CMD curl -f http://localhost:4000/ || exit 1

# Default command: serve with live reload
CMD ["bundle", "exec", "jekyll", "serve", \
     "--host", "0.0.0.0", \
     "--port", "4000", \
     "--livereload", \
     "--incremental", \
     "--force_polling"]

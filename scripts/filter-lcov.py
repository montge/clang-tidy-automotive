#!/usr/bin/env python3
"""Filter LCOV coverage data to only include automotive source files.

Also rewrites paths from the LLVM tree symlink location to src/automotive/
to match what SonarCloud expects based on sonar.sources configuration.
"""

import re
import sys


def filter_lcov(input_file, output_file, pattern="clang-tidy/automotive"):
    """Filter LCOV file to only include files matching pattern.

    Also rewrites paths from:
      .../llvm-project-.../clang-tools-extra/clang-tidy/automotive/...
    to:
      src/automotive/...
    """
    with open(input_file, 'r') as f:
        lines = f.readlines()

    output_lines = []
    include_record = False
    current_record = []

    # Pattern to match and rewrite the source file path
    path_pattern = re.compile(r'.*/clang-tools-extra/clang-tidy/automotive/(.*)')

    for line in lines:
        if line.startswith('SF:'):
            # Start of new source file record
            if include_record and current_record:
                output_lines.extend(current_record)

            include_record = pattern in line

            if include_record:
                # Rewrite the path to match sonar.sources
                match = path_pattern.match(line[3:].strip())
                if match:
                    new_path = f"SF:src/automotive/{match.group(1)}\n"
                    current_record = [new_path]
                else:
                    current_record = [line]
            else:
                current_record = [line]
        elif line.strip() == 'end_of_record':
            current_record.append(line)
            if include_record:
                output_lines.extend(current_record)
            current_record = []
            include_record = False
        else:
            current_record.append(line)

    # Handle last record
    if include_record and current_record:
        output_lines.extend(current_record)

    with open(output_file, 'w') as f:
        f.writelines(output_lines)

    return len([l for l in output_lines if l.startswith('SF:')])


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: filter-lcov.py <input.lcov> <output.lcov> [pattern]")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    pattern = sys.argv[3] if len(sys.argv) > 3 else "clang-tidy/automotive"

    count = filter_lcov(input_file, output_file, pattern)
    print(f"Filtered {count} source files matching '{pattern}'")

#!/usr/bin/env python3
"""Filter LCOV coverage data to only include automotive source files.

Also rewrites paths from the LLVM tree symlink location to src/automotive/
to match what SonarCloud expects based on sonar.sources configuration.

Additionally handles LCOV_EXCL_START/STOP markers to exclude lines from
coverage calculation.
"""

import os
import re
import sys


def read_excl_lines(source_path):
    """Read a source file and return set of line numbers to exclude.

    Handles LCOV_EXCL_LINE, LCOV_EXCL_START/STOP markers.
    """
    excl_lines = set()

    # Get project root from environment or determine from script location
    project_root = os.environ.get('PROJECT_ROOT',
                                   os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

    # Try multiple path variations
    paths_to_try = [
        os.path.join(project_root, source_path),
        source_path,
        source_path.replace('src/automotive/', ''),
    ]

    for path in paths_to_try:
        if os.path.exists(path):
            try:
                with open(path, 'r') as f:
                    in_excl_block = False
                    for line_num, line in enumerate(f, 1):
                        if 'LCOV_EXCL_START' in line:
                            in_excl_block = True
                        if in_excl_block or 'LCOV_EXCL_LINE' in line:
                            excl_lines.add(line_num)
                        if 'LCOV_EXCL_STOP' in line:
                            in_excl_block = False
                return excl_lines
            except Exception:
                pass

    return excl_lines


def filter_lcov(input_file, output_file, pattern="clang-tidy/automotive"):
    """Filter LCOV file to only include files matching pattern.

    Also rewrites paths from:
      .../llvm-project-.../clang-tools-extra/clang-tidy/automotive/...
    to:
      src/automotive/...

    And handles LCOV_EXCL markers by marking excluded lines as covered.
    """
    with open(input_file, 'r') as f:
        lines = f.readlines()

    output_lines = []
    include_record = False
    current_record = []
    current_source = None
    excl_lines = set()
    lines_hit = 0  # Count of lines with hit count > 0

    # Pattern to match and rewrite the source file path
    path_pattern = re.compile(r'.*/clang-tools-extra/clang-tidy/automotive/(.*)')

    for line in lines:
        if line.startswith('SF:'):
            # Start of new source file record
            if include_record and current_record:
                output_lines.extend(current_record)

            include_record = pattern in line
            lines_hit = 0

            if include_record:
                # Rewrite the path to match sonar.sources
                match = path_pattern.match(line[3:].strip())
                if match:
                    new_path = f"src/automotive/{match.group(1)}"
                    current_source = new_path
                    current_record = [f"SF:{new_path}\n"]
                    # Read exclusion markers from source file
                    excl_lines = read_excl_lines(new_path)
                else:
                    current_source = line[3:].strip()
                    current_record = [line]
                    excl_lines = set()
            else:
                current_record = [line]
                excl_lines = set()
        elif line.startswith('DA:'):
            # Data line: DA:line_number,hit_count
            if include_record:
                parts = line[3:].strip().split(',')
                if len(parts) >= 2:
                    line_num = int(parts[0])
                    hit_count = int(parts[1])
                    if line_num in excl_lines:
                        # Mark excluded lines as covered (hit count = 1)
                        current_record.append(f"DA:{line_num},1\n")
                        lines_hit += 1
                    else:
                        current_record.append(line)
                        if hit_count > 0:
                            lines_hit += 1
                else:
                    current_record.append(line)
            else:
                current_record.append(line)
        elif line.startswith('LH:'):
            # Update lines hit count with our recalculated value
            if include_record:
                current_record.append(f"LH:{lines_hit}\n")
            else:
                current_record.append(line)
        elif line.strip() == 'end_of_record':
            current_record.append(line)
            if include_record:
                output_lines.extend(current_record)
            current_record = []
            include_record = False
            excl_lines = set()
            lines_hit = 0
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

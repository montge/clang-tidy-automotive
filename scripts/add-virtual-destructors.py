#!/usr/bin/env python3
"""Add explicit virtual destructors to clang-tidy check headers.

This script adds `~ClassName() override = default;` to check classes
to satisfy SonarCloud's cpp:S1235 rule about virtual destructors.
"""

import re
import sys
from pathlib import Path


def add_destructor_to_file(filepath: Path) -> bool:
    """Add virtual destructor to a check header file if missing.

    Returns True if file was modified.
    """
    content = filepath.read_text()
    original_content = content

    # Skip if already has destructor for Check class
    if re.search(r'~\w+Check\s*\(\s*\)\s*(override)?\s*(=\s*default)?\s*;', content):
        return False

    # Find class name (ends with Check)
    class_match = re.search(r'class\s+(\w+Check)\s*:', content)
    if not class_match:
        return False

    class_name = class_match.group(1)

    # Find the constructor - it's usually in form:
    # ClassName(StringRef Name, ClangTidyContext *Context)
    #     : ClangTidyCheck(Name, Context) {}
    # OR
    # ClassName(StringRef Name, ClangTidyContext *Context);

    # Pattern for inline constructor with initializer list
    ctor_pattern = rf'({class_name}\s*\([^)]*\)\s*\n\s*:\s*\w+\([^)]*\)\s*\{{\}})'

    def add_dtor(match):
        return match.group(1) + f'\n\n  /// Destructor.\n  ~{class_name}() override = default;'

    new_content = re.sub(ctor_pattern, add_dtor, content)

    if new_content == original_content:
        # Try alternate pattern - constructor declaration only
        ctor_pattern2 = rf'({class_name}\s*\([^)]*\)\s*;)'
        new_content = re.sub(ctor_pattern2, add_dtor, content)

    if new_content != original_content:
        filepath.write_text(new_content)
        return True

    return False


def main():
    src_dir = Path(__file__).parent.parent / 'src' / 'automotive'

    modified = 0
    skipped = 0
    failed = []

    for header in sorted(src_dir.rglob('*Check.h')):
        # Skip utility headers
        if 'utils' in str(header):
            continue

        try:
            if add_destructor_to_file(header):
                print(f"Modified: {header.relative_to(src_dir)}")
                modified += 1
            else:
                skipped += 1
        except Exception as e:
            failed.append((header, str(e)))

    print(f"\nModified {modified} files, skipped {skipped} files")
    if failed:
        print(f"Failed {len(failed)} files:")
        for f, e in failed:
            print(f"  {f}: {e}")

    return 0


if __name__ == '__main__':
    sys.exit(main())

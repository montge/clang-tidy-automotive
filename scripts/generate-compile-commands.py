#!/usr/bin/env python3
"""
Generate compile_commands.json for automotive module source files.
This allows SonarCloud to analyze our C++ code without building LLVM.
"""

import json
import os
import glob
from pathlib import Path

def main():
    # Get project root (script is in scripts/)
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.absolute()

    # Find all C++ source files in src/automotive
    src_dir = project_root / "src" / "automotive"
    cpp_files = list(src_dir.rglob("*.cpp"))

    # Common compiler flags for LLVM/Clang development
    common_flags = [
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-fno-exceptions",
        "-fno-rtti",
        "-D__STDC_CONSTANT_MACROS",
        "-D__STDC_FORMAT_MACROS",
        "-D__STDC_LIMIT_MACROS",
    ]

    # Include paths (using system LLVM if available)
    include_paths = [
        "-I/usr/lib/llvm-20/include",
        "-I/usr/include/llvm-20",
        "-I/usr/include/clang-20",
        f"-I{project_root}/src",
    ]

    compile_commands = []

    for cpp_file in cpp_files:
        entry = {
            "directory": str(project_root),
            "file": str(cpp_file),
            "arguments": [
                "clang++",
                *common_flags,
                *include_paths,
                "-c",
                str(cpp_file),
                "-o",
                str(cpp_file.with_suffix(".o"))
            ]
        }
        compile_commands.append(entry)

    # Write compile_commands.json
    output_file = project_root / "compile_commands.json"
    with open(output_file, "w") as f:
        json.dump(compile_commands, f, indent=2)

    print(f"Generated {output_file} with {len(compile_commands)} entries")

if __name__ == "__main__":
    main()

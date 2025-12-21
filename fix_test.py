#!/usr/bin/env python3
import subprocess
import re
import sys

# Run clang-tidy to get actual output
test_file = sys.argv[1]
check_name = sys.argv[2] if len(sys.argv) > 2 else "automotive-*"

result = subprocess.run(
    [
        "./build/bin/clang-tidy",
        f"--checks=-*,{check_name}",
        test_file,
        "--",
    ],
    capture_output=True,
    text=True,
)

# Combine stdout and stderr
output = result.stdout + result.stderr

# Parse warnings
warnings = {}
for line in output.split("\n"):
    match = re.match(r"^(.*?):(\d+):(\d+): warning: (.+?) \[(.+?)\]$", line)
    if match:
        file_path, line_num, col_num, msg, check = match.groups()
        warnings[int(line_num)] = (int(col_num), msg, check)

# Read test file
with open(test_file, "r") as f:
    lines = f.readlines()

# Update CHECK-MESSAGES lines
updated_lines = []
for i, line in enumerate(lines, 1):
    # Check if this line has a CHECK-MESSAGES comment
    check_match = re.search(r"// CHECK-MESSAGES: :\[\[@LINE\+(\d+)\]\]:(\d+): warning: (.+?) \[(.+?)\]", line)
    if check_match:
        offset = int(check_match.group(1))
        old_col = int(check_match.group(2))
        expected_msg = check_match.group(3)
        expected_check = check_match.group(4)

        # Find the corresponding line number
        target_line = i + offset
        if target_line in warnings:
            actual_col, actual_msg, actual_check = warnings[target_line]
            # Update the column number
            new_line = line.replace(f"]:{old_col}:", f"]:{actual_col}:")
            updated_lines.append(new_line)
            print(f"Line {i}: Updated column from {old_col} to {actual_col}")
        else:
            updated_lines.append(line)
            print(f"Line {i}: No warning found for target line {target_line}")
    else:
        updated_lines.append(line)

# Write updated file
with open(test_file, "w") as f:
    f.writelines(updated_lines)

print(f"Updated {test_file}")

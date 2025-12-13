#!/usr/bin/env python3
"""
Convert clang-tidy output to SonarQube Generic Issue Import format.

This format is supported by SonarQube on-premise installations without requiring
additional plugins. It uses the Generic Issue Data format.

Usage:
    clang-tidy file.c --checks="automotive-*" -- 2>&1 | python3 clang-tidy-to-sonarqube.py > issues.json
    python3 clang-tidy-to-sonarqube.py --input clang-tidy-output.txt --output issues.json

For SonarQube configuration, add to sonar-project.properties:
    sonar.externalIssuesReportPaths=issues.json

Reference:
    https://docs.sonarqube.org/latest/analysis/generic-issue/
"""

import argparse
import json
import os
import re
import sys
from pathlib import Path
from typing import Optional

# MISRA rule mapping for severity and type
MISRA_RULE_MAP = {
    # Statement checks
    "automotive-avoid-goto": {
        "misra_rule": "15.1",
        "category": "Advisory",
        "type": "CODE_SMELL",
        "effort": "10min"
    },
    "automotive-missing-default-in-switch-stmt": {
        "misra_rule": "16.4",
        "category": "Required",
        "type": "BUG",
        "effort": "5min"
    },
    "automotive-avoid-continue": {
        "misra_rule": "15.5",
        "category": "Advisory",
        "type": "CODE_SMELL",
        "effort": "15min"
    },
    # Expression checks
    "automotive-avoid-comma-operator": {
        "misra_rule": "12.3",
        "category": "Advisory",
        "type": "CODE_SMELL",
        "effort": "5min"
    },
    "automotive-avoid-increment-decrement-in-expression": {
        "misra_rule": "13.3",
        "category": "Advisory",
        "type": "CODE_SMELL",
        "effort": "10min"
    },
    # Pointer checks
    "automotive-avoid-multiple-levels-of-pointer-indirection": {
        "misra_rule": "18.5",
        "category": "Advisory",
        "type": "CODE_SMELL",
        "effort": "30min"
    },
    # Type checks
    "automotive-wrong-bitfield-type": {
        "misra_rule": "6.1",
        "category": "Required",
        "type": "BUG",
        "effort": "5min"
    },
    "automotive-wrong-char-type-for-numeric": {
        "misra_rule": "10.1",
        "category": "Required",
        "type": "BUG",
        "effort": "10min"
    },
    # Literal checks
    "automotive-wrong-unsigned-literal-suffix": {
        "misra_rule": "7.2",
        "category": "Required",
        "type": "BUG",
        "effort": "2min"
    },
    "automotive-wrong-lowercase-literal-suffix": {
        "misra_rule": "7.3",
        "category": "Required",
        "type": "CODE_SMELL",
        "effort": "1min"
    },
    "automotive-avoid-octal-literal": {
        "misra_rule": "7.1",
        "category": "Required",
        "type": "BUG",
        "effort": "5min"
    },
    # Return checks
    "automotive-missing-return-in-non-void-function": {
        "misra_rule": "17.4",
        "category": "Mandatory",
        "type": "BUG",
        "effort": "10min"
    },
    "automotive-avoid-return-in-void-function": {
        "misra_rule": "17.4",
        "category": "Advisory",
        "type": "CODE_SMELL",
        "effort": "5min"
    },
    # Stdlib checks
    "automotive-avoid-dynamic-memory": {
        "misra_rule": "21.3",
        "category": "Required",
        "type": "VULNERABILITY",
        "effort": "1h"
    },
    "automotive-avoid-setjmp-longjmp": {
        "misra_rule": "21.4",
        "category": "Required",
        "type": "BUG",
        "effort": "2h"
    },
    "automotive-avoid-signal-handler": {
        "misra_rule": "21.5",
        "category": "Required",
        "type": "VULNERABILITY",
        "effort": "1h"
    },
    # Preprocessor checks
    "automotive-avoid-undef": {
        "misra_rule": "20.5",
        "category": "Advisory",
        "type": "CODE_SMELL",
        "effort": "5min"
    },
    # Array checks
    "automotive-avoid-flexible-array-member": {
        "misra_rule": "18.7",
        "category": "Required",
        "type": "BUG",
        "effort": "30min"
    },
    # Function checks
    "automotive-avoid-variable-length-array": {
        "misra_rule": "18.8",
        "category": "Required",
        "type": "VULNERABILITY",
        "effort": "20min"
    },
    "automotive-missing-static-on-internal-function": {
        "misra_rule": "8.8",
        "category": "Required",
        "type": "CODE_SMELL",
        "effort": "2min"
    },
    # Comment checks
    "automotive-avoid-c-comment-in-cpp-comment": {
        "misra_rule": "3.1",
        "category": "Required",
        "type": "CODE_SMELL",
        "effort": "5min"
    },
    # Storage checks
    "automotive-avoid-external-object-without-declaration": {
        "misra_rule": "8.4",
        "category": "Required",
        "type": "BUG",
        "effort": "10min"
    },
}


def get_severity_from_category(category: str) -> str:
    """Map MISRA category to SonarQube severity."""
    mapping = {
        "Mandatory": "BLOCKER",
        "Required": "CRITICAL",
        "Advisory": "MAJOR"
    }
    return mapping.get(category, "MAJOR")


def parse_clang_tidy_line(line: str) -> Optional[dict]:
    """
    Parse a clang-tidy diagnostic line.

    Format: file:line:col: severity: message [check-name]
    """
    pattern = r'^(.+?):(\d+):(\d+):\s*(warning|error|note):\s*(.+?)\s*\[([^\]]+)\]$'
    match = re.match(pattern, line.strip())

    if not match:
        return None

    filepath, line_num, col, severity, message, check_name = match.groups()

    return {
        "file": filepath,
        "line": int(line_num),
        "column": int(col),
        "severity": severity,
        "message": message,
        "check": check_name
    }


def create_sonarqube_issue(diagnostic: dict, base_path: str = "") -> dict:
    """Create a SonarQube generic issue from a parsed diagnostic."""
    check_name = diagnostic["check"]
    rule_info = MISRA_RULE_MAP.get(check_name, {})

    category = rule_info.get("category", "Advisory")
    misra_rule = rule_info.get("misra_rule", "")
    issue_type = rule_info.get("type", "CODE_SMELL")
    effort = rule_info.get("effort", "10min")

    # Normalize file path
    filepath = diagnostic["file"]
    if base_path and filepath.startswith(base_path):
        filepath = filepath[len(base_path):].lstrip("/")

    # Build message with MISRA reference
    message = diagnostic["message"]
    if misra_rule:
        message = f"[MISRA Rule {misra_rule}] {message}"

    issue = {
        "engineId": "clang-tidy-automotive",
        "ruleId": check_name,
        "severity": get_severity_from_category(category),
        "type": issue_type,
        "primaryLocation": {
            "message": message,
            "filePath": filepath,
            "textRange": {
                "startLine": diagnostic["line"],
                "startColumn": diagnostic["column"] - 1  # SonarQube uses 0-based columns
            }
        },
        "effortMinutes": parse_effort_to_minutes(effort)
    }

    return issue


def parse_effort_to_minutes(effort: str) -> int:
    """Convert effort string to minutes."""
    effort = effort.lower()
    if "h" in effort:
        return int(effort.replace("h", "")) * 60
    elif "min" in effort:
        return int(effort.replace("min", ""))
    return 10


def convert_to_sonarqube(diagnostics: list, base_path: str = "") -> dict:
    """Convert diagnostics to SonarQube Generic Issue format."""
    issues = [create_sonarqube_issue(d, base_path) for d in diagnostics]

    return {"issues": issues}


def main():
    parser = argparse.ArgumentParser(
        description="Convert clang-tidy output to SonarQube Generic Issue format"
    )
    parser.add_argument(
        "-i", "--input",
        type=str,
        help="Input file containing clang-tidy output (default: stdin)"
    )
    parser.add_argument(
        "-o", "--output",
        type=str,
        help="Output JSON file (default: stdout)"
    )
    parser.add_argument(
        "--base-path",
        type=str,
        default="",
        help="Base path to strip from file paths"
    )
    parser.add_argument(
        "--pretty",
        action="store_true",
        help="Pretty-print JSON output"
    )

    args = parser.parse_args()

    # Read input
    if args.input:
        with open(args.input, "r") as f:
            lines = f.readlines()
    else:
        lines = sys.stdin.readlines()

    # Parse diagnostics
    diagnostics = []
    for line in lines:
        diag = parse_clang_tidy_line(line)
        if diag:
            diagnostics.append(diag)

    # Convert to SonarQube format
    result = convert_to_sonarqube(diagnostics, args.base_path)

    # Output
    indent = 2 if args.pretty else None
    output_str = json.dumps(result, indent=indent)

    if args.output:
        with open(args.output, "w") as f:
            f.write(output_str)
    else:
        print(output_str)

    return 0


if __name__ == "__main__":
    sys.exit(main())

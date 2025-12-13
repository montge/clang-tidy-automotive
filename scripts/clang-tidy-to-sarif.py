#!/usr/bin/env python3
"""
Convert clang-tidy output to SARIF 2.1.0 format for SonarQube integration.

Usage:
    clang-tidy file.c --checks="automotive-*" -- 2>&1 | python3 clang-tidy-to-sarif.py > results.sarif
    python3 clang-tidy-to-sarif.py --input clang-tidy-output.txt --output results.sarif

The SARIF output can be imported into:
- SonarQube (via Generic Issue Import or SARIF import plugins)
- SonarCloud
- GitHub Code Scanning
- Azure DevOps
"""

import argparse
import json
import re
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Optional

# SARIF schema version
SARIF_VERSION = "2.1.0"
SARIF_SCHEMA = "https://raw.githubusercontent.com/oasis-tcs/sarif-spec/master/Schemata/sarif-schema-2.1.0.json"

# Mapping from automotive check names to MISRA rules
# This provides rich metadata in SARIF output
MISRA_RULE_MAP = {
    # Statement checks
    "automotive-avoid-goto": {
        "misra_rule": "15.1",
        "category": "Advisory",
        "title": "The goto statement should not be used",
        "description": "Use of goto can lead to unstructured code that is difficult to understand and maintain."
    },
    "automotive-missing-default-in-switch-stmt": {
        "misra_rule": "16.4",
        "category": "Required",
        "title": "Every switch statement shall have a default label",
        "description": "A default label ensures that all possible values are handled."
    },
    "automotive-avoid-continue": {
        "misra_rule": "15.5",
        "category": "Advisory",
        "title": "A function should have a single point of exit at the end",
        "description": "Multiple exit points can make control flow harder to understand."
    },
    # Expression checks
    "automotive-avoid-comma-operator": {
        "misra_rule": "12.3",
        "category": "Advisory",
        "title": "The comma operator should not be used",
        "description": "The comma operator can make code confusing and error-prone."
    },
    "automotive-avoid-increment-decrement-in-expression": {
        "misra_rule": "13.3",
        "category": "Advisory",
        "title": "A full expression containing an increment or decrement operator should have no other potential side effects",
        "description": "Mixing side effects can lead to undefined or unspecified behavior."
    },
    # Pointer checks
    "automotive-avoid-multiple-levels-of-pointer-indirection": {
        "misra_rule": "18.5",
        "category": "Advisory",
        "title": "Declarations should contain no more than two levels of pointer nesting",
        "description": "Deep pointer nesting makes code harder to understand and more error-prone."
    },
    # Type checks
    "automotive-wrong-bitfield-type": {
        "misra_rule": "6.1",
        "category": "Required",
        "title": "Bit-fields shall only be declared with an appropriate type",
        "description": "Bit-fields should use explicitly signed/unsigned integer types."
    },
    "automotive-wrong-char-type-for-numeric": {
        "misra_rule": "10.1",
        "category": "Required",
        "title": "Operands shall not be of an inappropriate essential type",
        "description": "Character types should not be used for numeric operations."
    },
    # Literal checks
    "automotive-wrong-unsigned-literal-suffix": {
        "misra_rule": "7.2",
        "category": "Required",
        "title": "A u or U suffix shall be applied to all integer constants that are unsigned",
        "description": "Explicit suffixes prevent implicit type conversions."
    },
    "automotive-wrong-lowercase-literal-suffix": {
        "misra_rule": "7.3",
        "category": "Required",
        "title": "The lowercase character l shall not be used in a literal suffix",
        "description": "Lowercase l can be confused with digit 1."
    },
    "automotive-avoid-octal-literal": {
        "misra_rule": "7.1",
        "category": "Required",
        "title": "Octal constants shall not be used",
        "description": "Octal notation can be confusing and error-prone."
    },
    # Return checks
    "automotive-missing-return-in-non-void-function": {
        "misra_rule": "17.4",
        "category": "Mandatory",
        "title": "All exit paths from a function with non-void return type shall have an explicit return statement",
        "description": "Missing return statements lead to undefined behavior."
    },
    "automotive-avoid-return-in-void-function": {
        "misra_rule": "17.4",
        "category": "Advisory",
        "title": "Return statement with expression in void function",
        "description": "Void functions should not return values."
    },
    # Stdlib checks
    "automotive-avoid-dynamic-memory": {
        "misra_rule": "21.3",
        "category": "Required",
        "title": "The memory allocation and deallocation functions shall not be used",
        "description": "Dynamic memory allocation can lead to memory leaks and fragmentation."
    },
    "automotive-avoid-setjmp-longjmp": {
        "misra_rule": "21.4",
        "category": "Required",
        "title": "The standard header file <setjmp.h> shall not be used",
        "description": "setjmp/longjmp bypass normal control flow and stack unwinding."
    },
    "automotive-avoid-signal-handler": {
        "misra_rule": "21.5",
        "category": "Required",
        "title": "The standard header file <signal.h> shall not be used",
        "description": "Signal handling has undefined behavior in many contexts."
    },
    # Preprocessor checks
    "automotive-avoid-undef": {
        "misra_rule": "20.5",
        "category": "Advisory",
        "title": "#undef should not be used",
        "description": "Undefining macros can lead to confusion about macro state."
    },
    # Array checks
    "automotive-avoid-flexible-array-member": {
        "misra_rule": "18.7",
        "category": "Required",
        "title": "Flexible array members shall not be declared",
        "description": "Flexible array members have limited portability and safety issues."
    },
    # Function checks
    "automotive-avoid-variable-length-array": {
        "misra_rule": "18.8",
        "category": "Required",
        "title": "Variable-length array types shall not be used",
        "description": "VLAs can cause stack overflow and have portability issues."
    },
    "automotive-missing-static-on-internal-function": {
        "misra_rule": "8.8",
        "category": "Required",
        "title": "If a function has internal linkage then it shall be declared with the static keyword",
        "description": "Internal functions should be explicitly marked static."
    },
    # Comment checks
    "automotive-avoid-c-comment-in-cpp-comment": {
        "misra_rule": "3.1",
        "category": "Required",
        "title": "The character sequences /* and // shall not be used within a comment",
        "description": "Nested comments can cause confusion."
    },
    # Storage checks
    "automotive-avoid-external-object-without-declaration": {
        "misra_rule": "8.4",
        "category": "Required",
        "title": "A compatible declaration shall be visible when an object or function is defined",
        "description": "All external objects should have visible declarations."
    },
}


def get_severity_from_category(category: str) -> str:
    """Map MISRA category to SARIF severity level."""
    mapping = {
        "Mandatory": "error",
        "Required": "warning",
        "Advisory": "note"
    }
    return mapping.get(category, "warning")


def parse_clang_tidy_line(line: str) -> Optional[dict]:
    """
    Parse a clang-tidy diagnostic line.

    Format: file:line:col: severity: message [check-name]
    Example: test.c:10:5: warning: goto statement detected [automotive-avoid-goto]
    """
    # Pattern for clang-tidy output
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


def create_sarif_rule(check_name: str) -> dict:
    """Create a SARIF rule entry for a clang-tidy check."""
    rule_info = MISRA_RULE_MAP.get(check_name, {})

    misra_rule = rule_info.get("misra_rule", "")
    category = rule_info.get("category", "Advisory")
    title = rule_info.get("title", check_name)
    description = rule_info.get("description", f"Clang-tidy check: {check_name}")

    rule = {
        "id": check_name,
        "name": check_name.replace("-", " ").title().replace(" ", ""),
        "shortDescription": {
            "text": title
        },
        "fullDescription": {
            "text": description
        },
        "defaultConfiguration": {
            "level": get_severity_from_category(category)
        },
        "properties": {
            "tags": ["MISRA", "automotive", "safety"],
            "precision": "high"
        }
    }

    if misra_rule:
        rule["properties"]["misraRule"] = f"Rule {misra_rule}"
        rule["properties"]["misraCategory"] = category
        rule["helpUri"] = f"https://github.com/montge/clang-tidy-automotive#rule-{misra_rule.replace('.', '')}"

    return rule


def create_sarif_result(diagnostic: dict, rule_index: int) -> dict:
    """Create a SARIF result entry from a parsed diagnostic."""
    check_name = diagnostic["check"]
    rule_info = MISRA_RULE_MAP.get(check_name, {})
    category = rule_info.get("category", "Advisory")

    result = {
        "ruleId": check_name,
        "ruleIndex": rule_index,
        "level": get_severity_from_category(category),
        "message": {
            "text": diagnostic["message"]
        },
        "locations": [{
            "physicalLocation": {
                "artifactLocation": {
                    "uri": diagnostic["file"],
                    "uriBaseId": "%SRCROOT%"
                },
                "region": {
                    "startLine": diagnostic["line"],
                    "startColumn": diagnostic["column"]
                }
            }
        }]
    }

    # Add MISRA rule reference as a fingerprint
    if rule_info.get("misra_rule"):
        result["fingerprints"] = {
            "misraRule": f"Rule {rule_info['misra_rule']}"
        }

    return result


def convert_to_sarif(diagnostics: list) -> dict:
    """Convert a list of parsed diagnostics to SARIF format."""
    # Collect unique rules
    rules = {}
    for diag in diagnostics:
        check = diag["check"]
        if check not in rules:
            rules[check] = len(rules)

    # Build rules array
    rule_list = [create_sarif_rule(check) for check in rules.keys()]

    # Build results array
    results = [
        create_sarif_result(diag, rules[diag["check"]])
        for diag in diagnostics
    ]

    sarif = {
        "$schema": SARIF_SCHEMA,
        "version": SARIF_VERSION,
        "runs": [{
            "tool": {
                "driver": {
                    "name": "clang-tidy-automotive",
                    "version": "1.0.0",
                    "informationUri": "https://github.com/montge/clang-tidy-automotive",
                    "rules": rule_list,
                    "properties": {
                        "standard": "MISRA C:2025",
                        "organization": "clang-tidy-automotive"
                    }
                }
            },
            "results": results,
            "invocations": [{
                "executionSuccessful": True,
                "endTimeUtc": datetime.now(timezone.utc).isoformat()
            }]
        }]
    }

    return sarif


def main():
    parser = argparse.ArgumentParser(
        description="Convert clang-tidy output to SARIF 2.1.0 format"
    )
    parser.add_argument(
        "-i", "--input",
        type=str,
        help="Input file containing clang-tidy output (default: stdin)"
    )
    parser.add_argument(
        "-o", "--output",
        type=str,
        help="Output SARIF file (default: stdout)"
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

    # Convert to SARIF
    sarif = convert_to_sarif(diagnostics)

    # Output
    indent = 2 if args.pretty else None
    output_str = json.dumps(sarif, indent=indent)

    if args.output:
        with open(args.output, "w") as f:
            f.write(output_str)
    else:
        print(output_str)

    # Return non-zero if issues found
    return 1 if diagnostics else 0


if __name__ == "__main__":
    sys.exit(main())

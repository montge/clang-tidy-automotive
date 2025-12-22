#!/usr/bin/env python3
"""
Generate initial traceability matrix from MISRA requirements and implemented checks.
"""

import json
import os
import re
import subprocess
from pathlib import Path

def get_requirement_files(base_path: Path) -> dict:
    """Get all requirement files organized by standard."""
    requirements = {
        "misra-c-2025": {},
        "misra-cpp-2023": {}
    }

    for standard in requirements.keys():
        std_path = base_path / standard
        if std_path.exists():
            for f in sorted(std_path.glob("*.rst")):
                # Extract rule ID from filename (e.g., Rule_10_4.rst -> Rule_10_4)
                rule_id = f.stem
                requirements[standard][rule_id] = {
                    "file": str(f.relative_to(base_path.parent.parent)),
                    "check_ids": [],
                    "status": "missing",
                    "category": extract_category(f),
                    "decidability": extract_decidability(f),
                    "notes": ""
                }

    return requirements

def extract_category(filepath: Path) -> str:
    """Extract category (mandatory/required/advisory) from requirement file."""
    try:
        content = filepath.read_text()
        if ":category: mandatory" in content.lower():
            return "mandatory"
        elif ":category: required" in content.lower():
            return "required"
        elif ":category: advisory" in content.lower():
            return "advisory"
    except:
        pass
    return "unknown"

def extract_decidability(filepath: Path) -> str:
    """Extract decidability from requirement file."""
    try:
        content = filepath.read_text()
        if ":decidability: decidable" in content.lower():
            return "decidable"
        elif ":decidability: undecidable" in content.lower():
            return "undecidable"
    except:
        pass
    return "unknown"

def get_implemented_checks(src_path: Path) -> dict:
    """Get all implemented checks by grepping source files."""
    checks = {}

    # Find all check IDs directly (handles multi-line registerCheck calls)
    result = subprocess.run(
        ["grep", "-roh", '"automotive-[^"]*"', "--include=*.cpp", str(src_path)],
        capture_output=True, text=True
    )

    for line in result.stdout.strip().split('\n'):
        if line:
            # Remove quotes
            check_id = line.strip('"')
            if check_id and check_id != "automotive-module":
                checks[check_id] = {
                    "mapped_requirements": []
                }

    return checks

def map_check_to_requirement(check_id: str) -> tuple:
    """Attempt to map a check ID to a requirement ID."""
    # Pattern: automotive-c23-req-10.4 -> Rule_10_4 in misra-c-2025
    # Pattern: automotive-cpp23-req-6.4 -> Rule_6_4_x in misra-cpp-2023

    patterns = [
        # C:2025 patterns
        (r"automotive-c2[35]-req-(\d+)\.(\d+)", "misra-c-2025", "Rule_{}_{}"),
        (r"automotive-c2[35]-adv-(\d+)\.(\d+)", "misra-c-2025", "Rule_{}_{}"),
        (r"automotive-c2[35]-mand-(\d+)\.(\d+)", "misra-c-2025", "Rule_{}_{}"),
        (r"automotive-c2[35]-req-dir-(\d+)\.(\d+)", "misra-c-2025", "Dir_{}_{}"),

        # C++:2023 patterns
        (r"automotive-cpp23-req-(\d+)\.(\d+)\.(\d+)", "misra-cpp-2023", "Rule_{}_{}_{}"),
        (r"automotive-cpp23-req-(\d+)\.(\d+)", "misra-cpp-2023", "Rule_{}_{}"),
        (r"automotive-cpp23-adv-(\d+)\.(\d+)\.(\d+)", "misra-cpp-2023", "Rule_{}_{}_{}"),
        (r"automotive-cpp23-adv-(\d+)\.(\d+)", "misra-cpp-2023", "Rule_{}_{}"),
    ]

    for pattern, standard, rule_format in patterns:
        match = re.match(pattern, check_id)
        if match:
            rule_id = rule_format.format(*match.groups())
            return (standard, rule_id)

    return (None, None)

def main():
    # Paths
    misra_docs_path = Path("/home/e/Development/MISRA-Docs/docs/requirements")
    src_path = Path("/home/e/Development/clang-tidy-automotive/src/automotive")
    output_path = Path("/home/e/Development/clang-tidy-automotive/config/misra-traceability-matrix.json")

    # Ensure config directory exists
    output_path.parent.mkdir(exist_ok=True)

    # Get requirements
    print("Collecting requirements...")
    requirements = get_requirement_files(misra_docs_path)

    # Get implemented checks
    print("Collecting implemented checks...")
    checks = get_implemented_checks(src_path)

    # Map checks to requirements
    print("Mapping checks to requirements...")
    mapped_count = 0
    for check_id in checks:
        standard, rule_id = map_check_to_requirement(check_id)
        if standard and rule_id:
            # Try exact match
            if rule_id in requirements.get(standard, {}):
                requirements[standard][rule_id]["check_ids"].append(check_id)
                requirements[standard][rule_id]["status"] = "implemented"
                checks[check_id]["mapped_requirements"].append(f"{standard}/{rule_id}")
                mapped_count += 1
            else:
                # For C++:2023, try to find matching sub-rules
                if standard == "misra-cpp-2023":
                    for req_id in requirements.get(standard, {}):
                        if req_id.startswith(rule_id.rsplit('_', 1)[0] + "_"):
                            # Close match for sub-rule
                            pass

    # Build output structure
    matrix = {
        "version": "1.0.0",
        "generated": str(Path(__file__).name),
        "statistics": {
            "total_requirements": sum(len(r) for r in requirements.values()),
            "total_checks": len(checks),
            "mapped_checks": mapped_count,
            "c2025_requirements": len(requirements["misra-c-2025"]),
            "cpp2023_requirements": len(requirements["misra-cpp-2023"]),
        },
        "requirements": requirements,
        "checks": checks
    }

    # Compute status counts
    status_counts = {"implemented": 0, "missing": 0, "excluded": 0}
    for std_reqs in requirements.values():
        for req in std_reqs.values():
            status_counts[req.get("status", "missing")] += 1
    matrix["statistics"]["status_counts"] = status_counts

    # Write output
    print(f"Writing matrix to {output_path}...")
    with open(output_path, 'w') as f:
        json.dump(matrix, f, indent=2)

    # Print summary
    print("\n=== Traceability Matrix Summary ===")
    print(f"Total requirements: {matrix['statistics']['total_requirements']}")
    print(f"  - MISRA C:2025: {matrix['statistics']['c2025_requirements']}")
    print(f"  - MISRA C++:2023: {matrix['statistics']['cpp2023_requirements']}")
    print(f"Total checks: {matrix['statistics']['total_checks']}")
    print(f"Mapped checks: {matrix['statistics']['mapped_checks']}")
    print(f"Status counts: {status_counts}")

if __name__ == "__main__":
    main()

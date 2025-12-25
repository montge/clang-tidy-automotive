#!/usr/bin/env python3
"""
Fix the traceability matrix by adding all missing check mappings.

This script adds mappings for legacy-named checks that weren't captured
by the original generator and sets proper decidability/exclusion reasons.
"""

import json
from pathlib import Path

# Complete mapping from MISRA-RULE-INVENTORY.md
# Format: (check_id, rule_number, standard)
LEGACY_CHECK_MAPPINGS = [
    # Statement checks
    ("automotive-avoid-goto", "15.1", "c2025"),
    ("automotive-forward-goto-label", "15.3", "c2025"),
    ("automotive-missing-break-in-case", "16.3", "c2025"),
    ("automotive-missing-default-in-switch", "16.4", "c2025"),
    ("automotive-avoid-boolean-in-switch", "16.7", "c2025"),
    ("automotive-x-req-16.2", "16.2", "c2025"),
    ("automotive-missing-compound", "15.6", "c2025"),
    ("automotive-missing-else", "15.7", "c2025"),
    ("automotive-unused-label", "2.6", "c2025"),

    # Literal checks
    ("automotive-avoid-octal-number", "7.1", "c2025"),
    ("automotive-avoid-lowercase-literal-suffix", "7.3", "c2025"),
    ("automotive-unterminated-escape-sequence", "4.1", "c2025"),

    # Operator checks
    ("automotive-avoid-comma-operator", "12.3", "c2025"),

    # Type checks
    ("automotive-avoid-union", "19.2", "c2025"),
    ("automotive-implicit-int", "8.1", "c2025"),
    ("automotive-unique-enum-value", "8.12", "c2025"),
    ("automotive-unused-type", "2.3", "c2025"),

    # Bitfield checks
    ("automotive-wrong-bitfield-type", "6.1", "c2025"),
    ("automotive-avoid-signed-single-bitfield", "6.2", "c2025"),
    ("automotive-avoid-bitfield-in-union", "6.3", "c2025"),  # Also covers 19.2

    # Pointer checks
    ("automotive-wrong-null-pointer-value", "11.9", "c2025"),

    # Array checks
    ("automotive-avoid-partial-array-init", "9.3", "c2025"),
    ("automotive-avoid-flexible-array-member", "18.7", "c2025"),
    ("automotive-avoid-variable-length-array", "18.8", "c2025"),
    ("automotive-missing-external-array-size", "8.11", "c2025"),

    # Function checks
    ("automotive-uncomplete-function-prototype", "8.2", "c2025"),
    ("automotive-function-declaration-mismatch", "8.3", "c2025"),
    ("automotive-missing-static-inline", "8.10", "c2025"),
    ("automotive-implicit-function-decl", "17.3", "c2025"),
    ("automotive-avoid-function-parameter-modification", "17.8", "c2025"),
    ("automotive-avoid-multiple-return-stmt", "15.5", "c2025"),
    ("automotive-missing-return-value-handling", "17.7", "c2025"),
    ("automotive-missing-return-void", "17.4", "c2025"),

    # Storage checks
    ("automotive-avoid-restrict-type", "8.14", "c2025"),

    # Preprocessor checks
    ("automotive-avoid-code-before-include", "20.1", "c2025"),
    ("automotive-avoid-invalid-header-char", "20.2", "c2025"),
    ("automotive-avoid-macro-named-as-ckeyword", "20.4", "c2025"),
    ("automotive-avoid-undef", "20.5", "c2025"),
    ("automotive-avoid-hash-operator", "20.10", "c2025"),
    ("automotive-avoid-multiple-hash-operators", "20.11", "c2025"),
    ("automotive-avoid-reserved-macro-identifier", "21.1", "c2025"),
    ("automotive-unused-macro", "2.5", "c2025"),

    # Comment checks
    ("automotive-avoid-comment-within-comment", "3.1", "c2025"),
    ("automotive-avoid-line-splicing-within-comment", "3.2", "c2025"),

    # Stdlib checks
    ("automotive-avoid-stdarg-header", "17.1", "c2025"),
    ("automotive-avoid-setjmp-header", "21.4", "c2025"),
    ("automotive-avoid-signal-header", "21.5", "c2025"),
    ("automotive-avoid-ascii-to-number", "21.7", "c2025"),
    ("automotive-avoid-stdlib-exit", "21.8", "c2025"),
    ("automotive-avoid-stdlib-system", "21.8", "c2025"),  # Same rule as exit
    ("automotive-avoid-stdlib-rand", "21.12", "c2025"),
    ("automotive-c23-req-22.10", "22.10", "c2025"),  # Errno testing check
    ("automotive-c23-req-22.10", "22.4", "c2025"),  # Also covers 22.4 (errno test after errno-setting func)

    # Unused code checks
    ("automotive-x-adv-2.4", "2.4", "c2025"),

    # New mandatory checks (December 2025)
    ("automotive-c25-mand-7.5", "7.5", "c2025"),    # Integer constant macro form
    ("automotive-c25-mand-18.10", "18.10", "c2025"), # Pointers to VLA types
    ("automotive-c25-mand-21.22", "21.22", "c2025"), # Tgmath operand types

    # New required checks (December 2025)
    ("automotive-c25-req-8.8", "8.8", "c2025"),     # Static storage class
    ("automotive-c25-req-20.6", "20.6", "c2025"),   # Preprocessing directive in macro arg
    ("automotive-c25-req-23.8", "23.8", "c2025"),   # Generic default position
]

# MISRA C++:2023 check mappings from Cpp23Component.cpp
# Format: (check_id, rule_number, standard)
CPP23_CHECK_MAPPINGS = [
    # Required checks
    ("automotive-cpp23-req-8.2.1", "8.2.1", "cpp2023"),   # C-style casts
    ("automotive-cpp23-req-15.3", "15.3.1", "cpp2023"),   # Object slicing
    ("automotive-cpp23-req-15.7", "15.7.1", "cpp2023"),   # Virtual destructors
    ("automotive-cpp23-req-15.1.3", "15.1.3", "cpp2023"), # Explicit constructors
    ("automotive-cpp23-req-18.4.2", "18.4.2", "cpp2023"), # Move noexcept
    ("automotive-cpp23-req-8.4.1", "8.4.1", "cpp2023"),   # Narrowing conversions
    ("automotive-cpp23-req-15.0.1", "15.0.1", "cpp2023"), # Rule of Five
    ("automotive-cpp23-req-18.4.1", "18.4.1", "cpp2023"), # Destructors throwing
    ("automotive-cpp23-req-8.2.3", "8.2.3", "cpp2023"),   # const_cast
    ("automotive-cpp23-req-8.2.4", "8.2.4", "cpp2023"),   # reinterpret_cast
    ("automotive-cpp23-req-15.1.2", "15.1.2", "cpp2023"), # Conversion operators explicit
    ("automotive-cpp23-req-18.4.3", "18.4.3", "cpp2023"), # Throw in noexcept
    ("automotive-cpp23-req-8.3.1", "8.3.1", "cpp2023"),   # Implicit conversions
    ("automotive-cpp23-req-9.3", "9.3.1", "cpp2023"),     # Compound statement body
    ("automotive-cpp23-req-9.4", "9.4.1", "cpp2023"),     # Switch minimum clauses
    ("automotive-cpp23-req-12.3", "12.3.1", "cpp2023"),   # Union prohibition
    ("automotive-cpp23-req-18.1", "18.1.1", "cpp2023"),   # Exception prohibition
    ("automotive-cpp23-req-21.6", "21.6.1", "cpp2023"),   # Dynamic memory
    ("automotive-cpp23-req-21.10", "21.10.1", "cpp2023"), # ctime prohibition
    ("automotive-cpp23-req-6.7", "6.7.1", "cpp2023"),     # Float equality
    ("automotive-cpp23-req-0.1", "0.1.1", "cpp2023"),     # Implicit lambda capture
    ("automotive-cpp23-req-9.5", "9.5.1", "cpp2023"),     # Nested switch label
    ("automotive-cpp23-req-13.3", "13.3.1", "cpp2023"),   # By-value param modification
    ("automotive-cpp23-req-6.4", "6.4.1", "cpp2023"),     # Assignment in sub-expression
    ("automotive-cpp23-req-6.5", "6.5.1", "cpp2023"),     # Float loop counter
    ("automotive-cpp23-req-9.6", "9.6.1", "cpp2023"),     # Missing return
    ("automotive-cpp23-req-19.3", "19.3.1", "cpp2023"),   # realloc prohibition
    ("automotive-cpp23-req-9.2", "9.2.1", "cpp2023"),     # Auto var initialization
    ("automotive-cpp23-req-7.11", "7.11.1", "cpp2023"),   # Null literal assignment
    ("automotive-cpp23-req-6.2", "6.2.1", "cpp2023"),     # Identifier shadowing
    ("automotive-cpp23-req-7.0", "7.0.1", "cpp2023"),     # Character type mismatch
    ("automotive-cpp23-req-5.0", "5.0.1", "cpp2023"),     # Inline type definition
    ("automotive-cpp23-req-7.4", "7.4.1", "cpp2023"),     # Assignment result use

    # Advisory checks
    ("automotive-cpp23-adv-8.2.5", "8.2.5", "cpp2023"),   # dynamic_cast
    ("automotive-cpp23-adv-17.0.1", "17.0.1", "cpp2023"), # Template constraints
    ("automotive-cpp23-adv-17.1.1", "17.1.1", "cpp2023"), # Concept definitions
    ("automotive-cpp23-adv-11.3", "11.3.1", "cpp2023"),   # C-style cast (alias)
    ("automotive-cpp23-adv-4.1", "4.1.1", "cpp2023"),     # Character comparison
    ("automotive-cpp23-adv-6.0", "6.0.1", "cpp2023"),     # Uninitialized array
    ("automotive-cpp23-adv-16.5", "16.5.1", "cpp2023"),   # Overloaded logical operators
    ("automotive-cpp23-adv-8.0", "8.0.1", "cpp2023"),     # Single point of exit
    ("automotive-cpp23-adv-8.1", "8.1.1", "cpp2023"),     # Explicit lambda capture
    ("automotive-cpp23-adv-18.5", "18.5.1", "cpp2023"),   # Dynamic exception spec
    ("automotive-cpp23-adv-19.0", "19.0.1", "cpp2023"),   # std::addressof
    ("automotive-cpp23-adv-10.3", "10.3.1", "cpp2023"),   # Unnamed namespace
    ("automotive-cpp23-adv-6.8", "6.8.1", "cpp2023"),     # Break/continue placement
    ("automotive-cpp23-adv-8.19", "8.19.1", "cpp2023"),   # Noreturn return
    ("automotive-cpp23-adv-8.20", "8.20.1", "cpp2023"),   # Noreturn void return
    ("automotive-cpp23-adv-8.18", "8.18.1", "cpp2023"),   # Statement expression

    # Mandatory checks
    ("automotive-cpp23-mand-6.8.2", "6.8.2", "cpp2023"),  # Return local address

    # New checks (December 2025)
    ("automotive-cpp23-adv-10.1.2", "10.1.2", "cpp2023"),  # auto type restrictions
    ("automotive-cpp23-req-7.11.2", "7.11.2", "cpp2023"),  # nullptr enforcement
    ("automotive-cpp23-req-10.2.1", "10.2.1", "cpp2023"),  # Enum underlying type
    ("automotive-cpp23-req-9.6.4", "9.6.4", "cpp2023"),    # goto prohibition (alias)
    ("automotive-cpp23-req-5.13.2", "5.13.2", "cpp2023"),  # No octal literals (alias)
    ("automotive-cpp23-req-9.4.2", "9.4.2", "cpp2023"),    # Default in switch (alias)
    ("automotive-cpp23-req-9.4.1", "9.4.1", "cpp2023"),    # No fallthrough (alias)

    # Shared C/C++ checks - can be used for both standards
    # These C checks also apply to C++ per MISRA-CPP-2023-INVENTORY.md
    ("automotive-avoid-goto", "9.6.4", "cpp2023"),                    # No goto (shared)
    ("automotive-avoid-comma-operator", "8.19.1", "cpp2023"),         # No comma operator
    ("automotive-avoid-octal-number", "5.13.2", "cpp2023"),           # No octal literals
    ("automotive-missing-default-in-switch", "9.4.2", "cpp2023"),     # Default in switch
    ("automotive-missing-break-in-case", "9.4.1", "cpp2023"),         # No fallthrough
    ("automotive-missing-compound", "9.3.1", "cpp2023"),              # Compound statements (already cpp23-req-9.3)
    ("automotive-avoid-reserved-macro-identifier", "19.0.4", "cpp2023"),  # Reserved identifiers
]

# Multi-rule coverage: single checks that cover multiple rules
MULTI_RULE_MAPPINGS = [
    # Rule 10.6 check covers 10.6, 10.7, 10.8
    ("automotive-c23-req-10.6", ["10.7", "10.8"], "c2025"),
    # Rule 11.2 check covers 11.2, 11.5, 11.6, 11.7
    ("automotive-c23-req-11.2", ["11.5", "11.6", "11.7"], "c2025"),
    # Rule 13.1 check covers 13.1, 13.2, 13.3
    ("automotive-c23-req-13.1", ["13.2", "13.3"], "c2025"),
    # Rule 18.1 check covers 18.1, 18.2, 18.3, 18.5
    ("automotive-c23-req-18.1", ["18.2", "18.3", "18.5"], "c2025"),
]

# Rules that need manual review (cannot be fully automated)
MANUAL_REVIEW_RULES = {
    "c2025": [
        "Dir_1_1", "Dir_2_1", "Dir_3_1", "Dir_4_1", "Dir_4_3", "Dir_4_4",
        "Dir_4_6", "Dir_4_7", "Dir_4_9", "Dir_4_11", "Dir_4_12", "Dir_4_13",
        "Dir_4_14", "Dir_4_15", "Dir_5_1", "Dir_5_2", "Dir_5_3",
        # Rules requiring human judgment
        "Rule_5_1", "Rule_5_2", "Rule_5_3", "Rule_5_5",
    ],
}

# Undecidable rules (require dataflow/runtime analysis)
UNDECIDABLE_RULES = {
    "c2025": [
        "Rule_1_5",   # Obsolescent features - Clang warnings handle
        "Rule_2_2",   # Dead code detection
        "Rule_8_15",  # Declaration location - scope-dependent
        "Rule_9_7",   # Atomic initialization
        "Rule_17_9",  # Noreturn return analysis
        "Rule_19_1",  # Overlapping storage
        "Rule_19_3",  # Union member read before write
        "Rule_21_14", # memcmp on strings
        "Rule_21_16", # memcmp pointer comparison - semantic intent
        "Rule_21_17", # strcpy/strcat bounds - requires dataflow
        "Rule_21_18", # string.h size validation
        "Rule_21_20", # localeconv/getenv pointer const usage
        "Rule_21_25", # Memory sync ordering - concurrent model
        "Rule_21_26", # mtx_timedlock type - thread dataflow
        "Rule_22_2",  # free() on non-heap memory
        "Rule_22_3",  # File writable mode exclusivity
        "Rule_22_5",  # FILE pointer dereference tracking
        "Rule_22_6",  # Use after fclose
        "Rule_22_7",  # EOF testing after file operations
        "Rule_22_11", # Thread join/detach
        "Rule_22_12", # fflush stream state
        "Rule_22_14", # Memory stream flush/seek tracking
        "Rule_22_15", "Rule_22_16", "Rule_22_17", "Rule_22_18", "Rule_22_19",
        "Rule_22_20",
    ],
}

# Rules covered by Clang built-in warnings
CLANG_BUILTIN_RULES = {
    "c2025": [
        "Rule_1_1",   # Standard C syntax
        "Rule_1_3",   # Undefined behavior
        "Rule_2_1",   # Unreachable code (-Wunreachable-code)
        "Rule_8_5",   # One definition rule
        "Rule_10_3",  # Implicit narrowing (-Wimplicit-conversion)
        "Rule_11_1",  # Pointer/integer (-Wpointer-arith)
        "Rule_12_1",  # Operator precedence (-Wparentheses)
        "Rule_17_5",  # Array parameter (-Warray-parameter)
        "Rule_20_13", # Valid preprocessing directive - Clang syntax error
        "Rule_22_1",  # Resource leaks
    ],
}


def rule_to_matrix_key(rule_number: str) -> str:
    """Convert rule number like '15.1' to matrix key like 'Rule_15_1'."""
    parts = rule_number.split(".")
    if rule_number.startswith("Dir"):
        return rule_number.replace(".", "_").replace(" ", "_")
    return f"Rule_{'_'.join(parts)}"


def load_matrix(path: Path) -> dict:
    """Load the existing traceability matrix."""
    with open(path) as f:
        return json.load(f)


def save_matrix(path: Path, matrix: dict):
    """Save the updated traceability matrix."""
    with open(path, "w") as f:
        json.dump(matrix, f, indent=2)


def update_matrix(matrix: dict) -> dict:
    """Update the matrix with all missing mappings."""
    stats = {"added": 0, "updated": 0, "excluded": 0, "cpp_added": 0}

    c2025_reqs = matrix["requirements"]["misra-c-2025"]
    cpp2023_reqs = matrix["requirements"]["misra-cpp-2023"]

    # Add legacy check mappings for C
    for check_id, rule_num, standard in LEGACY_CHECK_MAPPINGS:
        if standard == "c2025":
            key = rule_to_matrix_key(rule_num)
            if key in c2025_reqs:
                if check_id not in c2025_reqs[key]["check_ids"]:
                    c2025_reqs[key]["check_ids"].append(check_id)
                    c2025_reqs[key]["status"] = "implemented"
                    c2025_reqs[key]["decidability"] = "decidable"
                    stats["added"] += 1
            else:
                print(f"Warning: C Rule {key} not found in matrix")

    # Add C++ check mappings
    for check_id, rule_num, standard in CPP23_CHECK_MAPPINGS:
        if standard == "cpp2023":
            key = rule_to_matrix_key(rule_num)
            if key in cpp2023_reqs:
                if check_id not in cpp2023_reqs[key]["check_ids"]:
                    cpp2023_reqs[key]["check_ids"].append(check_id)
                    cpp2023_reqs[key]["status"] = "implemented"
                    cpp2023_reqs[key]["decidability"] = "decidable"
                    stats["cpp_added"] += 1
            else:
                print(f"Warning: C++ Rule {key} not found in matrix")

    # Add multi-rule mappings (checks that cover multiple rules)
    for check_id, rule_nums, standard in MULTI_RULE_MAPPINGS:
        if standard == "c2025":
            for rule_num in rule_nums:
                key = rule_to_matrix_key(rule_num)
                if key in c2025_reqs:
                    if check_id not in c2025_reqs[key]["check_ids"]:
                        c2025_reqs[key]["check_ids"].append(check_id)
                        c2025_reqs[key]["status"] = "implemented"
                        c2025_reqs[key]["decidability"] = "decidable"
                        stats["added"] += 1
                else:
                    print(f"Warning: Rule {key} not found in matrix")

    # Mark manual review rules
    for key in MANUAL_REVIEW_RULES.get("c2025", []):
        if key in c2025_reqs:
            c2025_reqs[key]["status"] = "excluded"
            c2025_reqs[key]["decidability"] = "decidable"  # Decidable but needs human
            c2025_reqs[key]["notes"] = "Requires manual code review"
            stats["excluded"] += 1

    # Mark undecidable rules
    for key in UNDECIDABLE_RULES.get("c2025", []):
        if key in c2025_reqs:
            c2025_reqs[key]["status"] = "excluded"
            c2025_reqs[key]["decidability"] = "undecidable"
            c2025_reqs[key]["notes"] = "Requires dataflow/runtime analysis"
            stats["excluded"] += 1

    # Mark clang builtin rules
    for key in CLANG_BUILTIN_RULES.get("c2025", []):
        if key in c2025_reqs:
            c2025_reqs[key]["status"] = "excluded"
            c2025_reqs[key]["decidability"] = "decidable"
            c2025_reqs[key]["notes"] = "Covered by Clang built-in warning"
            stats["excluded"] += 1

    # Update statistics
    implemented = sum(1 for r in c2025_reqs.values() if r["status"] == "implemented")
    excluded = sum(1 for r in c2025_reqs.values() if r["status"] == "excluded")
    missing = sum(1 for r in c2025_reqs.values() if r["status"] == "missing")

    cpp_implemented = sum(1 for r in cpp2023_reqs.values() if r["status"] == "implemented")
    cpp_excluded = sum(1 for r in cpp2023_reqs.values() if r["status"] == "excluded")
    cpp_missing = sum(1 for r in cpp2023_reqs.values() if r["status"] == "missing")

    matrix["statistics"] = {
        "total_requirements": len(c2025_reqs) + len(cpp2023_reqs),
        "total_checks": 181,
        "mapped_checks": implemented + cpp_implemented,
        "c2025_requirements": len(c2025_reqs),
        "cpp2023_requirements": len(cpp2023_reqs),
        "status_counts": {
            "implemented": implemented + cpp_implemented,
            "missing": missing + cpp_missing,
            "excluded": excluded + cpp_excluded
        }
    }

    print(f"Updates: {stats}")
    print(f"C:2025 - Implemented: {implemented}, Excluded: {excluded}, Missing: {missing}")
    print(f"C++:2023 - Implemented: {cpp_implemented}, Excluded: {cpp_excluded}, Missing: {cpp_missing}")

    return matrix


def main():
    project_root = Path(__file__).parent.parent
    matrix_path = project_root / "config" / "misra-traceability-matrix.json"

    print(f"Loading matrix from {matrix_path}")
    matrix = load_matrix(matrix_path)

    print("Updating matrix with legacy check mappings...")
    matrix = update_matrix(matrix)

    print(f"Saving updated matrix to {matrix_path}")
    save_matrix(matrix_path, matrix)

    print("Done!")


if __name__ == "__main__":
    main()

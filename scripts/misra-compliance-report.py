#!/usr/bin/env python3
"""
Generate MISRA C:2025 compliance reports from clang-tidy-automotive analysis.

This script produces HTML and JSON compliance reports showing:
- Rule coverage (implemented vs total)
- Violations by category (Mandatory, Required, Advisory)
- Per-file violation counts
- Compliance percentage

Usage:
    python3 misra-compliance-report.py \
        --mapping config/misra-rule-mapping.json \
        --issues sonarqube-issues.json \
        --output compliance-report.html
"""

import argparse
import json
import sys
from collections import defaultdict
from datetime import datetime
from pathlib import Path
from typing import Optional

HTML_TEMPLATE = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MISRA C:2025 Compliance Report</title>
    <style>
        :root {{
            --bg-color: #1a1a2e;
            --card-bg: #16213e;
            --text-color: #eee;
            --accent-color: #0f3460;
            --success-color: #00b894;
            --warning-color: #fdcb6e;
            --error-color: #d63031;
            --border-color: #2d3748;
        }}
        * {{ box-sizing: border-box; margin: 0; padding: 0; }}
        body {{
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: var(--bg-color);
            color: var(--text-color);
            line-height: 1.6;
            padding: 2rem;
        }}
        .container {{ max-width: 1200px; margin: 0 auto; }}
        h1 {{ margin-bottom: 1rem; color: #fff; }}
        h2 {{ margin: 2rem 0 1rem; color: #ddd; border-bottom: 2px solid var(--accent-color); padding-bottom: 0.5rem; }}
        .meta {{ color: #888; margin-bottom: 2rem; }}
        .grid {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 1rem; margin-bottom: 2rem; }}
        .card {{
            background: var(--card-bg);
            border-radius: 8px;
            padding: 1.5rem;
            border: 1px solid var(--border-color);
        }}
        .card-title {{ font-size: 0.9rem; color: #888; margin-bottom: 0.5rem; }}
        .card-value {{ font-size: 2rem; font-weight: bold; }}
        .card-value.success {{ color: var(--success-color); }}
        .card-value.warning {{ color: var(--warning-color); }}
        .card-value.error {{ color: var(--error-color); }}
        .progress-bar {{
            background: var(--accent-color);
            border-radius: 4px;
            height: 8px;
            margin-top: 0.5rem;
            overflow: hidden;
        }}
        .progress-fill {{
            background: var(--success-color);
            height: 100%;
            transition: width 0.3s;
        }}
        table {{
            width: 100%;
            border-collapse: collapse;
            margin-bottom: 2rem;
        }}
        th, td {{
            padding: 0.75rem;
            text-align: left;
            border-bottom: 1px solid var(--border-color);
        }}
        th {{ background: var(--accent-color); }}
        tr:hover {{ background: rgba(255,255,255,0.05); }}
        .badge {{
            display: inline-block;
            padding: 0.25rem 0.5rem;
            border-radius: 4px;
            font-size: 0.75rem;
            font-weight: bold;
        }}
        .badge-mandatory {{ background: var(--error-color); }}
        .badge-required {{ background: var(--warning-color); color: #000; }}
        .badge-advisory {{ background: var(--success-color); color: #000; }}
        .badge-full {{ background: var(--success-color); color: #000; }}
        .badge-partial {{ background: var(--warning-color); color: #000; }}
        .badge-none {{ background: #666; }}
        .violations-list {{ max-height: 400px; overflow-y: auto; }}
        footer {{ margin-top: 3rem; padding-top: 1rem; border-top: 1px solid var(--border-color); color: #666; font-size: 0.875rem; }}
    </style>
</head>
<body>
    <div class="container">
        <h1>MISRA C:2025 Compliance Report</h1>
        <p class="meta">Generated: {generated_date} | Project: clang-tidy-automotive</p>

        <div class="grid">
            <div class="card">
                <div class="card-title">Compliance Score</div>
                <div class="card-value {compliance_class}">{compliance_score}%</div>
                <div class="progress-bar">
                    <div class="progress-fill" style="width: {compliance_score}%"></div>
                </div>
            </div>
            <div class="card">
                <div class="card-title">Rules Implemented</div>
                <div class="card-value">{rules_implemented} / {rules_total}</div>
            </div>
            <div class="card">
                <div class="card-title">Total Violations</div>
                <div class="card-value {violations_class}">{total_violations}</div>
            </div>
            <div class="card">
                <div class="card-title">Files Analyzed</div>
                <div class="card-value">{files_analyzed}</div>
            </div>
        </div>

        <h2>Violations by Category</h2>
        <div class="grid">
            <div class="card">
                <div class="card-title">Mandatory</div>
                <div class="card-value error">{mandatory_violations}</div>
            </div>
            <div class="card">
                <div class="card-title">Required</div>
                <div class="card-value warning">{required_violations}</div>
            </div>
            <div class="card">
                <div class="card-title">Advisory</div>
                <div class="card-value">{advisory_violations}</div>
            </div>
        </div>

        <h2>Rule Coverage</h2>
        <table>
            <thead>
                <tr>
                    <th>Check ID</th>
                    <th>MISRA Rule</th>
                    <th>Category</th>
                    <th>Coverage</th>
                    <th>Violations</th>
                </tr>
            </thead>
            <tbody>
                {rule_rows}
            </tbody>
        </table>

        <h2>Top Violations by File</h2>
        <table>
            <thead>
                <tr>
                    <th>File</th>
                    <th>Violations</th>
                    <th>Top Rule</th>
                </tr>
            </thead>
            <tbody>
                {file_rows}
            </tbody>
        </table>

        <footer>
            <p>Generated by clang-tidy-automotive | <a href="https://github.com/montge/clang-tidy-automotive" style="color: #888;">GitHub</a></p>
        </footer>
    </div>
</body>
</html>
"""


def load_mapping(mapping_file: str) -> dict:
    """Load the MISRA rule mapping file."""
    with open(mapping_file, "r") as f:
        return json.load(f)


def load_issues(issues_file: str) -> dict:
    """Load the SonarQube issues file."""
    if not Path(issues_file).exists():
        return {"issues": []}
    with open(issues_file, "r") as f:
        return json.load(f)


def get_category_badge(category: str) -> str:
    """Generate HTML badge for category."""
    css_class = f"badge-{category.lower()}"
    return f'<span class="badge {css_class}">{category}</span>'


def get_coverage_badge(coverage: str) -> str:
    """Generate HTML badge for coverage level."""
    css_class = f"badge-{coverage}"
    return f'<span class="badge {css_class}">{coverage.title()}</span>'


def generate_report(mapping: dict, issues: dict) -> str:
    """Generate HTML compliance report."""
    # Count violations by rule and category
    violations_by_rule = defaultdict(int)
    violations_by_category = defaultdict(int)
    violations_by_file = defaultdict(lambda: {"count": 0, "rules": defaultdict(int)})

    rules_map = {r["check_id"]: r for r in mapping.get("rules", [])}

    for issue in issues.get("issues", []):
        rule_id = issue.get("ruleId", "")
        file_path = issue.get("primaryLocation", {}).get("filePath", "unknown")

        violations_by_rule[rule_id] += 1
        violations_by_file[file_path]["count"] += 1
        violations_by_file[file_path]["rules"][rule_id] += 1

        if rule_id in rules_map:
            category = rules_map[rule_id].get("category", "Advisory")
            violations_by_category[category] += 1

    # Calculate metrics
    total_violations = sum(violations_by_rule.values())
    rules_implemented = len(mapping.get("rules", []))
    rules_total = rules_implemented + len(mapping.get("unimplemented_rules", []))
    files_analyzed = len(violations_by_file)

    # Compliance score (simplified: 100% if no mandatory/required violations)
    mandatory_count = violations_by_category.get("Mandatory", 0)
    required_count = violations_by_category.get("Required", 0)
    advisory_count = violations_by_category.get("Advisory", 0)

    if mandatory_count > 0:
        compliance_score = max(0, 50 - mandatory_count * 10)
    elif required_count > 0:
        compliance_score = max(50, 80 - required_count * 2)
    else:
        compliance_score = min(100, 95 + (5 if advisory_count == 0 else 0))

    # Generate rule rows
    rule_rows = []
    for rule in mapping.get("rules", []):
        check_id = rule["check_id"]
        misra_rule = rule.get("misra_rule", "")
        category = rule.get("category", "Advisory")
        coverage = rule.get("coverage", "full")
        violation_count = violations_by_rule.get(check_id, 0)

        row = f"""
        <tr>
            <td>{check_id}</td>
            <td>Rule {misra_rule}</td>
            <td>{get_category_badge(category)}</td>
            <td>{get_coverage_badge(coverage)}</td>
            <td>{violation_count}</td>
        </tr>
        """
        rule_rows.append(row)

    # Generate file rows (top 20)
    sorted_files = sorted(
        violations_by_file.items(),
        key=lambda x: x[1]["count"],
        reverse=True
    )[:20]

    file_rows = []
    for file_path, data in sorted_files:
        top_rule = max(data["rules"].items(), key=lambda x: x[1])[0] if data["rules"] else "-"
        row = f"""
        <tr>
            <td>{file_path}</td>
            <td>{data['count']}</td>
            <td>{top_rule}</td>
        </tr>
        """
        file_rows.append(row)

    # Determine CSS classes
    compliance_class = "success" if compliance_score >= 80 else ("warning" if compliance_score >= 50 else "error")
    violations_class = "success" if total_violations == 0 else ("warning" if total_violations < 50 else "error")

    return HTML_TEMPLATE.format(
        generated_date=datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        compliance_score=compliance_score,
        compliance_class=compliance_class,
        rules_implemented=rules_implemented,
        rules_total=rules_total,
        total_violations=total_violations,
        violations_class=violations_class,
        files_analyzed=files_analyzed,
        mandatory_violations=mandatory_count,
        required_violations=required_count,
        advisory_violations=advisory_count,
        rule_rows="".join(rule_rows),
        file_rows="".join(file_rows) if file_rows else "<tr><td colspan='3'>No violations found</td></tr>"
    )


def generate_json_report(mapping: dict, issues: dict) -> dict:
    """Generate JSON compliance report."""
    violations_by_rule = defaultdict(int)
    violations_by_category = defaultdict(int)

    rules_map = {r["check_id"]: r for r in mapping.get("rules", [])}

    for issue in issues.get("issues", []):
        rule_id = issue.get("ruleId", "")
        violations_by_rule[rule_id] += 1
        if rule_id in rules_map:
            category = rules_map[rule_id].get("category", "Advisory")
            violations_by_category[category] += 1

    return {
        "generated": datetime.now().isoformat(),
        "standard": "MISRA C:2025",
        "summary": {
            "rules_implemented": len(mapping.get("rules", [])),
            "rules_total": len(mapping.get("rules", [])) + len(mapping.get("unimplemented_rules", [])),
            "total_violations": sum(violations_by_rule.values()),
            "violations_by_category": dict(violations_by_category)
        },
        "rules": [
            {
                "check_id": r["check_id"],
                "misra_rule": r.get("misra_rule"),
                "category": r.get("category"),
                "coverage": r.get("coverage"),
                "violations": violations_by_rule.get(r["check_id"], 0)
            }
            for r in mapping.get("rules", [])
        ]
    }


def main():
    parser = argparse.ArgumentParser(
        description="Generate MISRA C:2025 compliance report"
    )
    parser.add_argument(
        "--mapping",
        type=str,
        default="config/misra-rule-mapping.json",
        help="Path to MISRA rule mapping file"
    )
    parser.add_argument(
        "--issues",
        type=str,
        default="sonarqube-issues.json",
        help="Path to SonarQube issues file"
    )
    parser.add_argument(
        "-o", "--output",
        type=str,
        help="Output file (default: stdout)"
    )
    parser.add_argument(
        "--format",
        choices=["html", "json"],
        default="html",
        help="Output format"
    )

    args = parser.parse_args()

    # Load data
    mapping = load_mapping(args.mapping)
    issues = load_issues(args.issues)

    # Generate report
    if args.format == "html":
        report = generate_report(mapping, issues)
    else:
        report = json.dumps(generate_json_report(mapping, issues), indent=2)

    # Output
    if args.output:
        with open(args.output, "w") as f:
            f.write(report)
        print(f"Report generated: {args.output}", file=sys.stderr)
    else:
        print(report)

    return 0


if __name__ == "__main__":
    sys.exit(main())

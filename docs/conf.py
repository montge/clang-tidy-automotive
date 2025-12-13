# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys

# -- Project information -----------------------------------------------------
project = 'Clang-Tidy Automotive'
copyright = '2024, clang-tidy-automotive contributors'
author = 'clang-tidy-automotive contributors'
release = '1.0.0'

# -- General configuration ---------------------------------------------------
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.viewcode',
    'sphinx.ext.todo',
    'sphinx_needs',
    'breathe',
    'myst_parser',
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

# -- Breathe configuration (Doxygen integration) -----------------------------
breathe_projects = {
    'automotive': '../docs/api/xml'
}
breathe_default_project = 'automotive'

# -- sphinx-needs configuration ----------------------------------------------
needs_types = [
    {
        'directive': 'misra-rule',
        'title': 'MISRA Rule',
        'prefix': 'MISRA_',
        'color': '#1E90FF',
        'style': 'node',
    },
    {
        'directive': 'misra-dir',
        'title': 'MISRA Directive',
        'prefix': 'DIR_',
        'color': '#4169E1',
        'style': 'node',
    },
    {
        'directive': 'check',
        'title': 'Check',
        'prefix': 'CHK_',
        'color': '#32CD32',
        'style': 'node',
    },
    {
        'directive': 'test',
        'title': 'Test',
        'prefix': 'TST_',
        'color': '#FFD700',
        'style': 'node',
    },
]

needs_extra_options = [
    'category',      # Required, Advisory, Mandatory
    'decidable',     # Yes, No
    'scope',         # Single TU, System
    'check_id',      # clang-tidy check ID
    'rule_text',     # Rule description (without MISRA copyright text)
    'status',        # Implemented, Partial, Not Implemented, Manual Review
]

needs_extra_links = [
    {
        'option': 'implements',
        'incoming': 'is_implemented_by',
        'outgoing': 'implements rule',
    },
    {
        'option': 'tests',
        'incoming': 'is_tested_by',
        'outgoing': 'tests',
    },
    {
        'option': 'verifies',
        'incoming': 'is_verified_by',
        'outgoing': 'verifies',
    },
]

# ID regex pattern for needs
needs_id_regex = r'^[A-Z][A-Z0-9_]*'

# Enable needs filtering and linking features
needs_build_json = True
needs_reproducible_json = True

# -- MyST configuration (Markdown support) -----------------------------------
myst_enable_extensions = [
    'colon_fence',
    'deflist',
]

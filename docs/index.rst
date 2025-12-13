Clang-Tidy Automotive Documentation
====================================

MISRA C:2025 compliance checks for safety-critical automotive software.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   getting-started
   misra-coverage
   checks/index
   api/index
   traceability

Overview
--------

Clang-Tidy Automotive provides static analysis checks for MISRA C:2025 compliance.
These checks integrate with the LLVM/Clang toolchain and can be used in CI/CD
pipelines, IDEs, and SonarQube for compliance reporting.

Key Features
~~~~~~~~~~~~

- **56+ MISRA checks** covering ~25% of MISRA C:2025 rules
- **Multi-version support** for Clang 17, 18, 19, and 20
- **SonarQube integration** for compliance reporting
- **Full traceability** from rules to checks to tests

Quick Start
~~~~~~~~~~~

.. code-block:: bash

   # Build with your project
   clang-tidy -checks="automotive-*" your_file.c --

   # Run specific rule category
   clang-tidy -checks="automotive-avoid-*" your_file.c --

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
* :ref:`needfilter`

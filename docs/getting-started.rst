Getting Started
===============

This guide covers building and using clang-tidy-automotive.

Prerequisites
-------------

- **Clang/LLVM 17-20** (one of the LTS versions)
- **CMake 3.20+**
- **Ninja** (recommended)
- **Python 3.8+** (for documentation)

Building
--------

1. Clone the repository:

   .. code-block:: bash

      git clone https://github.com/yourorg/clang-tidy-automotive.git
      cd clang-tidy-automotive

2. Download LLVM source:

   .. code-block:: bash

      ./download.sh

3. Set up symlinks:

   .. code-block:: bash

      ./setup.sh

4. Configure and build:

   .. code-block:: bash

      mkdir build && cd build
      cmake -G Ninja \
          -DCMAKE_CXX_COMPILER=clang++-20 \
          -DCMAKE_C_COMPILER=clang-20 \
          -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
          -DLLVM_TARGETS_TO_BUILD="X86" \
          ../llvm-project-llvmorg-20.1.8/llvm
      ninja clang-tidy

Running Checks
--------------

Basic Usage
~~~~~~~~~~~

.. code-block:: bash

   ./build/bin/clang-tidy your_file.c --checks="automotive-*" --

Specific Categories
~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Statement checks only
   ./build/bin/clang-tidy file.c --checks="automotive-avoid-goto,automotive-missing-*" --

   # Standard library checks
   ./build/bin/clang-tidy file.c --checks="automotive-avoid-stdlib-*" --

With Compile Commands
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Use compile_commands.json
   ./build/bin/clang-tidy -p build/ src/file.c --checks="automotive-*"

Configuration File
------------------

Create a ``.clang-tidy`` file in your project:

.. code-block:: yaml

   Checks: >
     automotive-*,
     -automotive-avoid-goto

   WarningsAsErrors: 'automotive-*'

   CheckOptions:
     # Add check-specific options here

CI Integration
--------------

GitHub Actions
~~~~~~~~~~~~~~

See ``.github/workflows/ci.yml`` for a complete example. Key steps:

.. code-block:: yaml

   - name: Run clang-tidy
     run: |
       find src -name '*.c' | xargs clang-tidy --checks="automotive-*" --

SonarQube
~~~~~~~~~

Export findings in SARIF format for SonarQube integration.
See :doc:`traceability` for compliance reporting.

Documentation
-------------

Build the documentation:

.. code-block:: bash

   cd docs
   pip install -r requirements-docs.txt
   make html

Generate Doxygen API docs:

.. code-block:: bash

   doxygen Doxyfile

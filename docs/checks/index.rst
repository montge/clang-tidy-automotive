Checks Reference
================

This section documents all implemented checks with their MISRA rule mappings.

.. toctree::
   :maxdepth: 1
   :caption: Check Categories:

   statements
   stdlib
   types
   preprocessor

Check Overview
--------------

.. needtable::
   :types: check
   :columns: id, title, implements, status
   :style: table
   :sort: id

Statement Checks
----------------

.. check:: CHK_AVOID_GOTO
   :title: Avoid Goto Statement
   :check_id: automotive-avoid-goto
   :status: Implemented
   :implements: MISRA_15_1

   Detects usage of goto statements in code.

   **clang-tidy check:** ``automotive-avoid-goto``

   .. code-block:: c

      void func(void) {
          goto label;  // Warning: goto statement detected
      label:
          return;
      }

Standard Library Checks
-----------------------

.. check:: CHK_AVOID_MALLOC
   :title: Avoid Dynamic Memory
   :check_id: automotive-avoid-stdlib-malloc-call
   :status: Implemented
   :implements: MISRA_21_3

   Detects usage of malloc, calloc, realloc, and free.

   **clang-tidy check:** ``automotive-avoid-stdlib-malloc-call``

   .. code-block:: c

      void func(void) {
          int *p = malloc(sizeof(int));  // Warning
          free(p);                       // Warning
      }

.. check:: CHK_AVOID_SETJMP
   :title: Avoid setjmp.h
   :check_id: automotive-avoid-setjmp-header
   :status: Implemented
   :implements: MISRA_21_4

   Detects inclusion of setjmp.h and usage of setjmp/longjmp.

   **clang-tidy check:** ``automotive-avoid-setjmp-header``

.. check:: CHK_AVOID_SIGNAL
   :title: Avoid signal.h
   :check_id: automotive-avoid-signal-header
   :status: Implemented
   :implements: MISRA_21_5

   Detects inclusion of signal.h and usage of signal functions.

   **clang-tidy check:** ``automotive-avoid-signal-header``

Type Checks
-----------

.. check:: CHK_SIGNED_BITFIELD
   :title: Signed Single-Bit Field
   :check_id: automotive-avoid-signed-single-bitfield
   :status: Implemented
   :implements: MISRA_6_2

   Detects single-bit bit-fields with signed types.

   **clang-tidy check:** ``automotive-avoid-signed-single-bitfield``

   .. code-block:: c

      struct Flags {
          signed int flag : 1;  // Warning: signed single-bit field
      };

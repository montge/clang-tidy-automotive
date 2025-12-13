Standard Library Checks
=======================

Checks for standard library usage (MISRA C:2025 Rules 21.x).

Dynamic Memory
--------------

.. check:: CHK_STDLIB_MALLOC
   :title: Avoid Dynamic Memory
   :check_id: automotive-avoid-stdlib-malloc-call
   :status: Implemented
   :implements: MISRA_21_3

   Detects usage of malloc, calloc, realloc, aligned_alloc, and free.

   **Check ID:** ``automotive-avoid-stdlib-malloc-call``

   Example violations:

   .. code-block:: c

      void func(void) {
          int *p = malloc(100);  // Warning
          p = realloc(p, 200);   // Warning
          free(p);               // Warning
      }

setjmp/longjmp
--------------

.. check:: CHK_STDLIB_SETJMP
   :title: Avoid setjmp.h
   :check_id: automotive-avoid-setjmp-header
   :status: Implemented
   :implements: MISRA_21_4

   Detects inclusion of <setjmp.h> and usage of setjmp/longjmp.

   **Check ID:** ``automotive-avoid-setjmp-header``

Signal Handling
---------------

.. check:: CHK_STDLIB_SIGNAL
   :title: Avoid signal.h
   :check_id: automotive-avoid-signal-header
   :status: Implemented
   :implements: MISRA_21_5

   Detects inclusion of <signal.h> and usage of signal functions.

   **Check ID:** ``automotive-avoid-signal-header``

String Conversion
-----------------

.. check:: CHK_STDLIB_ATOX
   :title: Avoid atoi/atof/atol
   :check_id: automotive-avoid-ascii-to-number
   :status: Implemented
   :implements: MISRA_21_7

   Detects usage of atoi, atof, atol, and atoll.

   **Check ID:** ``automotive-avoid-ascii-to-number``

   Use strtol/strtod instead for proper error handling.

Exit Functions
--------------

.. check:: CHK_STDLIB_EXIT
   :title: Avoid exit/abort
   :check_id: automotive-avoid-stdlib-exit-call
   :status: Implemented
   :implements: MISRA_21_8

   Detects usage of exit, abort, and _Exit functions.

   **Check ID:** ``automotive-avoid-stdlib-exit-call``

system() Function
-----------------

.. check:: CHK_STDLIB_SYSTEM
   :title: Avoid system()
   :check_id: automotive-avoid-stdlib-system-call
   :status: Implemented
   :implements: MISRA_21_8

   Detects usage of the system() function.

   **Check ID:** ``automotive-avoid-stdlib-system-call``

Random Numbers
--------------

.. check:: CHK_STDLIB_RAND
   :title: Avoid rand/srand
   :check_id: automotive-avoid-stdlib-rand-call
   :status: Implemented
   :implements: MISRA_21_24

   Detects usage of rand() and srand().

   **Check ID:** ``automotive-avoid-stdlib-rand-call``

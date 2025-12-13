Statement Checks
================

Checks for control statement rules (MISRA C:2025 Rules 15.x, 16.x).

Goto Statement
--------------

.. check:: CHK_STMT_GOTO
   :title: Avoid Goto
   :check_id: automotive-avoid-goto
   :status: Implemented
   :implements: MISRA_15_1

   Detects usage of goto statements.

   **Check ID:** ``automotive-avoid-goto``

   Example violation:

   .. code-block:: c

      void func(void) {
          goto error;  // Warning
      error:
          return;
      }

   Compliant alternative:

   .. code-block:: c

      void func(void) {
          if (condition) {
              handle_error();
              return;
          }
      }

Multiple Return Statements
--------------------------

.. check:: CHK_STMT_MULTI_RETURN
   :title: Multiple Return Statements
   :check_id: automotive-avoid-multiple-return-stmt
   :status: Implemented
   :implements: MISRA_15_5

   Detects functions with more than one return statement.

   **Check ID:** ``automotive-avoid-multiple-return-stmt``

Missing Default Case
--------------------

.. check:: CHK_STMT_DEFAULT
   :title: Missing Default in Switch
   :check_id: automotive-missing-default-in-switch
   :status: Implemented
   :implements: MISRA_16_4

   Detects switch statements without a default case.

   **Check ID:** ``automotive-missing-default-in-switch``

Missing Compound Statement
--------------------------

.. check:: CHK_STMT_COMPOUND
   :title: Missing Compound Statement
   :check_id: automotive-missing-compound
   :status: Implemented
   :implements: MISRA_15_6

   Detects control statements without braces.

   **Check ID:** ``automotive-missing-compound``

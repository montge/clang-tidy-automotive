Preprocessor Checks
===================

Checks for preprocessor rules (MISRA C:2025 Rules 20.x).

Keyword Macros
--------------

.. check:: CHK_PP_KEYWORD
   :title: Macro Named as Keyword
   :check_id: automotive-avoid-macro-named-as-ckeyword
   :status: Implemented
   :implements: MISRA_20_4

   Detects macro names that match C keywords.

   **Check ID:** ``automotive-avoid-macro-named-as-ckeyword``

   Example violation:

   .. code-block:: c

      #define if 0  // Warning: keyword macro

#undef Directive
----------------

.. check:: CHK_PP_UNDEF
   :title: Avoid #undef
   :check_id: automotive-avoid-undef
   :status: Implemented
   :implements: MISRA_20_5

   Detects usage of the #undef directive.

   **Check ID:** ``automotive-avoid-undef``

Stringification Operator
------------------------

.. check:: CHK_PP_HASH
   :title: Avoid # Operator
   :check_id: automotive-avoid-hash-operator
   :status: Implemented
   :implements: MISRA_20_10

   Detects usage of the # stringification operator.

   **Check ID:** ``automotive-avoid-hash-operator``

   Example violation:

   .. code-block:: c

      #define STR(x) #x  // Warning: # operator

Concatenation Operator
----------------------

.. check:: CHK_PP_CONCAT
   :title: Avoid ## Operator
   :check_id: automotive-avoid-multiple-hash-operators
   :status: Implemented
   :implements: MISRA_20_11

   Detects usage of the ## token concatenation operator.

   **Check ID:** ``automotive-avoid-multiple-hash-operators``

   Example violation:

   .. code-block:: c

      #define CONCAT(a, b) a##b  // Warning: ## operator

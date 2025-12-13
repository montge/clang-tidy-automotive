MISRA C:2025 Coverage
=====================

This document tracks the implementation status of MISRA C:2025 rules.

Coverage Summary
----------------

.. needtable::
   :types: misra-rule
   :columns: id, title, status, is_implemented_by
   :style: table
   :sort: id

Rules by Category
-----------------

Comments (Rules 3.x)
~~~~~~~~~~~~~~~~~~~~

.. misra-rule:: MISRA_3_1
   :title: Comment sequences within comments
   :category: Required
   :status: Implemented
   :rule_text: The character sequences /* and // shall not appear within a comment.

   Nested comment markers can cause unintended code to be commented out or activated.

   .. needtable::
      :filter: 'MISRA_3_1' in implements
      :columns: id, title

.. misra-rule:: MISRA_3_2
   :title: Line-splicing in // comments
   :category: Required
   :status: Implemented
   :rule_text: Line-splicing shall not be used in // comments.

   A backslash at the end of a // comment line continues the comment to the next line.

Types (Rules 6.x)
~~~~~~~~~~~~~~~~~

.. misra-rule:: MISRA_6_1
   :title: Bit-field types
   :category: Required
   :status: Implemented
   :rule_text: Bit-fields shall only be declared with an appropriate type.

   Only specific integer types should be used for bit-fields to ensure portability.

.. misra-rule:: MISRA_6_2
   :title: Signed single-bit fields
   :category: Required
   :status: Implemented
   :rule_text: Single-bit named bit-fields shall not be of a signed type.

   A signed single-bit field can only hold 0 or -1, which is rarely intended.

Literals (Rules 7.x)
~~~~~~~~~~~~~~~~~~~~

.. misra-rule:: MISRA_7_1
   :title: Octal constants
   :category: Required
   :status: Implemented
   :rule_text: Octal constants shall not be used.

   Octal constants (leading zero) are easily confused with decimal constants.

.. misra-rule:: MISRA_7_3
   :title: Lowercase literal suffix
   :category: Required
   :status: Implemented
   :rule_text: The lowercase character 'l' shall not be used in a literal suffix.

   Lowercase 'l' can be confused with digit '1'.

Declarations (Rules 8.x)
~~~~~~~~~~~~~~~~~~~~~~~~

.. misra-rule:: MISRA_8_1
   :title: Explicit types
   :category: Required
   :status: Implemented
   :rule_text: Types shall be explicitly specified.

   Implicit int and implicit function declarations are error-prone.

.. misra-rule:: MISRA_8_14
   :title: Restrict qualifier
   :category: Required
   :status: Implemented
   :rule_text: The restrict type qualifier shall not be used.

   Incorrect use of restrict leads to undefined behavior.

Statements (Rules 15.x)
~~~~~~~~~~~~~~~~~~~~~~~

.. misra-rule:: MISRA_15_1
   :title: Goto statement
   :category: Advisory
   :status: Implemented
   :rule_text: The goto statement should not be used.

   Goto leads to unstructured control flow that is hard to analyze.

Standard Library (Rules 21.x)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. misra-rule:: MISRA_21_3
   :title: Dynamic memory
   :category: Required
   :status: Implemented
   :rule_text: The memory allocation and deallocation functions shall not be used.

   Dynamic memory allocation is unpredictable and can lead to memory leaks.

.. misra-rule:: MISRA_21_4
   :title: setjmp.h
   :category: Required
   :status: Implemented
   :rule_text: The standard header file <setjmp.h> shall not be used.

   setjmp/longjmp bypass normal control flow and can cause resource leaks.

.. misra-rule:: MISRA_21_5
   :title: signal.h
   :category: Required
   :status: Implemented
   :rule_text: The standard header file <signal.h> shall not be used.

   Signal handlers have severe restrictions and are difficult to use correctly.

Not Yet Implemented
-------------------

The following rule categories have no implementation:

- **Rules 5.x** (Identifiers) - 9 rules
- **Rules 10.x** (Conversions) - 8 rules
- **Rules 22.x** (Resources) - 10 rules
- **Rules 23.x** (Atomics) - 4 rules

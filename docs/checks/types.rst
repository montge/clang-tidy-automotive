Type Checks
===========

Checks for type-related rules (MISRA C:2025 Rules 6.x, 8.x).

Bit-field Types
---------------

.. check:: CHK_TYPE_BITFIELD
   :title: Wrong Bit-field Type
   :check_id: automotive-wrong-bitfield-type
   :status: Implemented
   :implements: MISRA_6_1

   Detects bit-fields declared with inappropriate types.

   **Check ID:** ``automotive-wrong-bitfield-type``

Signed Single-Bit Fields
------------------------

.. check:: CHK_TYPE_SIGNED_BIT
   :title: Signed Single-Bit Field
   :check_id: automotive-avoid-signed-single-bitfield
   :status: Implemented
   :implements: MISRA_6_2

   Detects single-bit bit-fields with signed types.

   **Check ID:** ``automotive-avoid-signed-single-bitfield``

   Example violation:

   .. code-block:: c

      struct Flags {
          int flag : 1;  // Warning: signed single-bit
      };

   Compliant:

   .. code-block:: c

      struct Flags {
          unsigned int flag : 1;  // OK
          _Bool enable : 1;       // OK
      };

Implicit Int
------------

.. check:: CHK_TYPE_IMPLICIT_INT
   :title: Implicit Int
   :check_id: automotive-implict-int
   :status: Implemented
   :implements: MISRA_8_1

   Detects declarations that implicitly use int type.

   **Check ID:** ``automotive-implict-int``

Union Types
-----------

.. check:: CHK_TYPE_UNION
   :title: Avoid Union
   :check_id: automotive-avoid-union
   :status: Implemented
   :implements: MISRA_19_2

   Detects usage of union types.

   **Check ID:** ``automotive-avoid-union``

Bit-field in Union
------------------

.. check:: CHK_TYPE_BIT_UNION
   :title: Bit-field in Union
   :check_id: automotive-avoid-bitfield-in-union
   :status: Implemented
   :implements: MISRA_19_2

   Detects bit-field members within unions.

   **Check ID:** ``automotive-avoid-bitfield-in-union``

Traceability Matrix
===================

This page provides traceability between MISRA rules, implemented checks, and tests.

Rule to Check Mapping
---------------------

.. needflow::
   :types: misra-rule, check
   :show_link_names:

Coverage Metrics
----------------

.. needpie::
   :types: misra-rule
   :labels: Implemented, Partial, Not Implemented
   :filter: status == 'Implemented', status == 'Partial', status == 'Not Implemented'

Check Implementation Status
---------------------------

.. needtable::
   :types: check
   :columns: id, title, check_id, implements, status
   :style: table
   :sort: id

Filtering Examples
------------------

Required Rules Only
~~~~~~~~~~~~~~~~~~~

.. needtable::
   :types: misra-rule
   :filter: category == 'Required'
   :columns: id, title, status
   :style: table

Implemented Checks
~~~~~~~~~~~~~~~~~~

.. needtable::
   :types: check
   :filter: status == 'Implemented'
   :columns: id, title, check_id
   :style: table

Rules Without Checks
~~~~~~~~~~~~~~~~~~~~

.. needtable::
   :types: misra-rule
   :filter: status == 'Not Implemented'
   :columns: id, title, category
   :style: table

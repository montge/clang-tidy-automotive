
add_clang_library(clangTidyAutomotiveArrayComponent STATIC
  ArrayComponent.cpp
  AvoidDesignatedInitializerBoundsCheck.cpp
  AvoidFlexibleArrayMemberCheck.cpp
  AvoidIncompleteInitializationCheck.cpp
  AvoidPartialArrayInitCheck.cpp
  AvoidPointerArithmeticBoundsCheck.cpp
  AvoidPointerArithmeticCheck.cpp
  AvoidVariableLengthArrayCheck.cpp
  MissingExternalArraySizeCheck.cpp
  PartialArrayInitCheck.cpp
)

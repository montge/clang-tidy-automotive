
add_clang_library(clangTidyAutomotiveArrayComponent STATIC
  ArrayComponent.cpp
  AvoidFlexibleArrayMemberCheck.cpp
  AvoidPartialArrayInitCheck.cpp
  AvoidPointerArithmeticBoundsCheck.cpp
  AvoidPointerArithmeticCheck.cpp
  AvoidVariableLengthArrayCheck.cpp
  MissingExternalArraySizeCheck.cpp
)

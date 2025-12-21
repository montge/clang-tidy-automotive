
add_clang_library(clangTidyAutomotivePointerComponent STATIC
  AvoidAtomicVoidPointerCheck.cpp
  AvoidAutoAddressEscapeCheck.cpp
  AvoidCastRemovingQualifierCheck.cpp
  AvoidImplicitNullComparisonCheck.cpp
  AvoidIncompatiblePointerCastCheck.cpp
  AvoidIncompatiblePointerConversionCheck.cpp
  AvoidPointerIntegerCastCheck.cpp
  PointerComponent.cpp
  WrongNullPointerValueCheck.cpp
)

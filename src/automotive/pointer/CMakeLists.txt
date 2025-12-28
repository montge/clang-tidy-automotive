add_clang_library(clangTidyAutomotivePointerComponent STATIC
  AvoidAtomicVoidPointerCheck.cpp
  AvoidAutoAddressEscapeCheck.cpp
  AvoidCastRemovingQualifierCheck.cpp
  AvoidConstQualifierRemovalCheck.cpp
  AvoidImplicitNullComparisonCheck.cpp
  AvoidIncompatiblePointerCastCheck.cpp
  AvoidIncompatiblePointerConversionCheck.cpp
  AvoidPointerArithmeticC25Check.cpp
  AvoidPointerIntegerCastCheck.cpp
  AvoidPointerToFloatCastCheck.cpp
  AvoidPointerTypeCastCheck.cpp
  PointerComponent.cpp
  WrongNullPointerValueCheck.cpp
  WrongPointerArithmeticCheck.cpp
)


add_clang_library(clangTidyAutomotiveTypeComponent STATIC
  AvoidAtomicVoidCheck.cpp
  AvoidPointerTypedefCheck.cpp
  AvoidUnionCheck.cpp
  DuplicateTagNameCheck.cpp
  DuplicateTypedefNameCheck.cpp
  ExplicitEnumeratorValuesCheck.cpp
  ImplicitIntCheck.cpp
  TypeComponent.cpp
  UniqueEnumValuesCheck.cpp
  UnusedTypeCheck.cpp
)

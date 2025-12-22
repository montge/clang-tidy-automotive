
add_clang_library(clangTidyAutomotiveStorageComponent STATIC
  AvoidGlobalObjectCheck.cpp
  AvoidMultipleAlignmentCheck.cpp
  AvoidZeroAlignmentCheck.cpp
  AvoidReservedIdentifierDeclarationCheck.cpp
  AvoidRestrictTypeCheck.cpp
  AvoidTentativeDefinitionInHeaderCheck.cpp
  AvoidUninitializedReadCheck.cpp
  DuplicateExternalIdentifierCheck.cpp
  ExternalSymbolCollectorCheck.cpp
  MissingStaticForInternalCheck.cpp
  MultipleExternalDefinitionCheck.cpp
  StorageComponent.cpp
)

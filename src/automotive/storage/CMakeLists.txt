
add_clang_library(clangTidyAutomotiveStorageComponent STATIC
  AvoidExternInSourceFileCheck.cpp
  AvoidGlobalObjectCheck.cpp
  AvoidMultipleAlignmentCheck.cpp
  AvoidZeroAlignmentCheck.cpp
  AvoidReservedIdentifierDeclarationCheck.cpp
  AvoidRestrictTypeCheck.cpp
  AvoidTentativeDefinitionInHeaderCheck.cpp
  AvoidUninitializedReadCheck.cpp
  DuplicateExternalIdentifierCheck.cpp
  ExternalSymbolCollectorCheck.cpp
  MissingPriorDeclarationCheck.cpp
  MissingStaticForInternalCheck.cpp
  MultipleExternalDefinitionCheck.cpp
  StaticStorageClassCheck.cpp
  StorageComponent.cpp
  ThreadObjectStorageCheck.cpp
)

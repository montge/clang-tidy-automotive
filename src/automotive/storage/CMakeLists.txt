
add_clang_library(clangTidyAutomotiveStorageComponent STATIC
  AvoidGlobalObjectCheck.cpp
  AvoidMultipleAlignmentCheck.cpp
  AvoidZeroAlignmentCheck.cpp
  AvoidReservedIdentifierDeclarationCheck.cpp
  AvoidRestrictTypeCheck.cpp
  AvoidTentativeDefinitionInHeaderCheck.cpp
  AvoidUninitializedReadCheck.cpp
  MissingStaticForInternalCheck.cpp
  StorageComponent.cpp
)


add_clang_library(clangTidyAutomotiveStorageComponent STATIC
  AvoidGlobalObjectCheck.cpp
  AvoidReservedIdentifierDeclarationCheck.cpp
  AvoidRestrictTypeCheck.cpp
  AvoidTentativeDefinitionInHeaderCheck.cpp
  AvoidUninitializedReadCheck.cpp
  MissingStaticForInternalCheck.cpp
  StorageComponent.cpp
)

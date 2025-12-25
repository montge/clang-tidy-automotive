
add_clang_library(clangTidyAutomotiveBitfieldComponent STATIC
  AvoidBitfieldInUnionCheck.cpp
  AvoidSignedSingleBitFieldCheck.cpp
  BitfieldComponent.cpp
  BitfieldInUnionCheck.cpp
  WrongBitfieldTypeCheck.cpp
)

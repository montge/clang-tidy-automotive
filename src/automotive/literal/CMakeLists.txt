
add_clang_library(clangTidyAutomotiveLiteralComponent STATIC
  AvoidLowercaseLiteralSuffixCheck.cpp
  AvoidOctalNumberCheck.cpp
  AvoidSmallIntConstMacroCheck.cpp
  AvoidStringLiteralToNonConstCheck.cpp
  AvoidTrigraphCheck.cpp
  AvoidUnsignedLiteralWithoutSuffixCheck.cpp
  LiteralComponent.cpp
  UnterminatedEscapeSequenceCheck.cpp
)

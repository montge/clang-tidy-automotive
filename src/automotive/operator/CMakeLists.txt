
add_clang_library(clangTidyAutomotiveOperatorComponent STATIC
  AvoidCommaOperatorCheck.cpp
  ExplicitPrecedenceCheck.cpp
  OperatorComponent.cpp
  WrongConditionalOperandTypeCheck.cpp
  WrongShiftOperandCheck.cpp
)

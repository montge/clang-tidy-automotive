
add_clang_library(clangTidyAutomotiveCpp23Component STATIC
  AvoidConstCastCheck.cpp
  AvoidCStyleCastCheck.cpp
  AvoidDynamicCastCheck.cpp
  AvoidImplicitConversionCheck.cpp
  AvoidImplicitConversionOperatorCheck.cpp
  AvoidNarrowingConversionCheck.cpp
  AvoidReinterpretCastCheck.cpp
  AvoidSlicingCheck.cpp
  AvoidThrowInNoexceptCheck.cpp
  AvoidThrowingDestructorCheck.cpp
  Cpp23Component.cpp
  ExplicitConstructorCheck.cpp
  NoexceptMoveCheck.cpp
  ProperConceptDefinitionCheck.cpp
  RuleOfFiveCheck.cpp
  UnconstrainedTemplateCheck.cpp
  VirtualDestructorCheck.cpp
)

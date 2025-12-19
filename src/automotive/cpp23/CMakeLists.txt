
add_clang_library(clangTidyAutomotiveCpp23Component STATIC
  AvoidCStyleCastCheck.cpp
  AvoidDynamicCastCheck.cpp
  AvoidNarrowingConversionCheck.cpp
  AvoidSlicingCheck.cpp
  AvoidThrowingDestructorCheck.cpp
  Cpp23Component.cpp
  ExplicitConstructorCheck.cpp
  NoexceptMoveCheck.cpp
  RuleOfFiveCheck.cpp
  VirtualDestructorCheck.cpp
)


add_clang_library(clangTidyAutomotiveCpp23Component STATIC
  AvoidCStyleCastCheck.cpp
  AvoidSlicingCheck.cpp
  Cpp23Component.cpp
  ExplicitConstructorCheck.cpp
  NoexceptMoveCheck.cpp
  VirtualDestructorCheck.cpp
)


add_clang_library(clangTidyAutomotiveExpressionComponent STATIC
  AvoidAtomicMemberAccessCheck.cpp
  AvoidAssignmentInExpressionCheck.cpp
  AvoidCharacterArithmeticCheck.cpp
  AvoidCompositeExpressionMismatchCheck.cpp
  AvoidConstantWrapAroundCheck.cpp
  AvoidEssentialTypeMismatchCheck.cpp
  AvoidInappropriateCastCheck.cpp
  AvoidInappropriateEssentialTypeCheck.cpp
  AvoidNarrowingCompoundAssignmentCheck.cpp
  AvoidNonBooleanInConditionCheck.cpp
  AvoidSideEffectInInitializerCheck.cpp
  AvoidSideEffectInLogicalOperandCheck.cpp
  AvoidSideEffectInSizeofCheck.cpp
  AvoidSizeofArrayParameterCheck.cpp
  ExpressionComponent.cpp
  GenericAppropriateTypeCheck.cpp
  GenericDefaultPositionCheck.cpp
  GenericFromMacroCheck.cpp
  GenericImplicitPointerCheck.cpp
  GenericNonDefaultCheck.cpp
  GenericSideEffectCheck.cpp
  GenericEssentialTypeCheck.cpp
  InvariantControlCheck.cpp
)

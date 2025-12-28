
add_clang_library(clangTidyAutomotiveExpressionComponent STATIC
  AvoidAssignmentInExpressionCheck.cpp
  AvoidAtomicMemberAccessCheck.cpp
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
  GenericEssentialTypeCheck.cpp
  GenericFromMacroCheck.cpp
  GenericImplicitPointerCheck.cpp
  GenericNonDefaultCheck.cpp
  GenericSideEffectCheck.cpp
  InvariantControlCheck.cpp
)

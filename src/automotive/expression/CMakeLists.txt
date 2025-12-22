
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
  InvariantControlCheck.cpp
)

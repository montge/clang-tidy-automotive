
add_clang_library(clangTidyAutomotiveFunctionComponent STATIC
  AvoidFunctionParameterModificationCheck.cpp
  AvoidNoreturnNonVoidCheck.cpp
  AvoidQualifiedFunctionTypeCheck.cpp
  AvoidStaticInArrayParamCheck.cpp
  FunctionComponent.cpp
  FunctionDeclarationMismatchCheck.cpp
  FunctionIdentifierUsageCheck.cpp
  ImplicitFunctionDeclCheck.cpp
  ObsolescentFeaturesCheck.cpp
  StaticInlineCheck.cpp
  UncompleteFunctionPrototypeCheck.cpp
)

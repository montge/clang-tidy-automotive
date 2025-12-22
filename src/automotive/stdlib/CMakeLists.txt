
add_clang_library(clangTidyAutomotiveStdlibComponent STATIC
  AtoXCheck.cpp
  AvoidApiCheck.cpp
  AvoidBsearchQsortCheck.cpp
  AvoidCtypeHeaderCheck.cpp
  AvoidSetjmpHeaderCheck.cpp
  AvoidSignalHeaderCheck.cpp
  AvoidStdargHeaderCheck.cpp
  AvoidStdioHeaderCheck.cpp
  AvoidStdlibMemoryCheck.cpp
  AvoidStdlibRandCheck.cpp
  AvoidstdlibsystemcallCheck.cpp
  AvoidTgmathHeaderCheck.cpp
  AvoidTimeHeaderCheck.cpp
  AvoidWcharHeaderCheck.cpp
  ErrnoTestingCheck.cpp
  ExitCheck.cpp
  StdlibComponent.cpp
)

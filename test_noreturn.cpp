// Simple test file for AvoidNoreturnReturnCheck
[[noreturn]] void testFunction() {
  return;  // This should trigger the check
}

[[noreturn]] void compliantFunction() {
  __builtin_trap();  // This is OK
}

int main() {
  return 0;
}

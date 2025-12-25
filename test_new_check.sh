#!/bin/bash
# Test script for the new AvoidForLoopVarModificationCheck

# Create a simple test file
cat > /tmp/test_loop_mod.cpp << 'EOF'
void test() {
  for (int i = 0; i < 10; i++) {
    i++;  // Should trigger warning
  }

  for (int j = 0; j < 10; j++) {
    int k = j;  // Should NOT trigger warning
    k++;
  }
}
EOF

# Run clang-tidy with the new check
./build/bin/clang-tidy --checks="automotive-cpp23-req-9.5.1" /tmp/test_loop_mod.cpp --

echo "Test complete. Check output above for warnings."

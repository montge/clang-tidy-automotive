# MISRA C++:2023 Rule 16.6.1 Implementation

## Rule Description
A variadic template shall have at least one instantiation with at least one variadic argument.

**Category:** Advisory
**Check Name:** `automotive-cpp23-adv-16.6.1`

## Rationale
Variadic templates that are never used with variadic arguments are unnecessarily complex. Such templates should use regular (non-variadic) templates instead to reduce complexity and improve code clarity.

## Implementation

### Files Created
1. **Header:** `/home/e/Development/clang-tidy-automotive/src/automotive/cpp23/AvoidUnusedVariadicTemplateCheck.h`
2. **Implementation:** `/home/e/Development/clang-tidy-automotive/src/automotive/cpp23/AvoidUnusedVariadicTemplateCheck.cpp`
3. **Test:** `/home/e/Development/clang-tidy-automotive/test/checkers/automotive/cpp23/avoid-unused-variadic-template.cpp`
4. **Examples:**
   - Violations: `/home/e/Development/clang-tidy-automotive/examples/cpp23/avoid-unused-variadic-template-violation.cpp`
   - Compliant: `/home/e/Development/clang-tidy-automotive/examples/cpp23/avoid-unused-variadic-template-compliant.cpp`

### Registration
- Added to `Cpp23Component.cpp` with check ID `automotive-cpp23-adv-16.6.1`
- Added to `CMakeLists.txt` for build integration

## Check Behavior

### What It Detects
The check tracks variadic template declarations and their instantiations across a translation unit:

1. **Function Templates:**
   - Identifies function templates with parameter packs (e.g., `template<typename... Args>`)
   - Tracks all instantiations of these templates
   - Warns if all instantiations have empty parameter packs

2. **Class Templates:**
   - Identifies class templates with parameter packs
   - Tracks template specializations
   - Warns if the pack is never expanded with multiple arguments

### Algorithm
1. **Declaration Phase:** Records all variadic template declarations (both function and class templates)
2. **Instantiation Phase:** For each instantiation, counts how many arguments came from the parameter pack
3. **End of Translation Unit:** Reports variadic templates that:
   - Have at least one instantiation, AND
   - Never have a variadic instantiation (pack always empty)

### Detection Logic
The check counts "fixed" template parameters vs. total template arguments:
- If `total_args > fixed_params`, the parameter pack was expanded with arguments
- Handles both type and non-type template parameters
- Supports mixed templates (e.g., `template<typename T, typename... Rest>`)

## Examples

### Violation
```cpp
// WARNING: Never used with variadic arguments
template<typename... Args>
void unused_variadic(Args... args) {}

void foo() {
  unused_variadic();  // Only called with empty pack
}
```

### Compliant
```cpp
// OK: Used with variadic arguments
template<typename... Args>
void properly_used(Args... args) {}

void foo() {
  properly_used(1, 2, 3);  // Called with multiple arguments
}

// OK: Regular template (non-variadic)
template<typename T>
void single_arg(T arg) {}
```

## Limitations

1. **Single Translation Unit Analysis:**
   - The check only analyzes instantiations within the current translation unit
   - Templates defined in headers but instantiated elsewhere may be reported incorrectly
   - This is a common limitation of clang-tidy checks

2. **Heuristic Approach:**
   - The check uses a parameter counting heuristic to detect pack expansion
   - May not catch all complex template metaprogramming patterns

3. **Advisory Category:**
   - As an advisory rule, this check provides guidance but doesn't enforce mandatory compliance
   - Legitimate use cases may exist for variadic templates with optional empty packs

## Building and Testing

### Build
```bash
./build.sh
```

### Run Test
```bash
./build/bin/clang-tidy \
  --checks="automotive-cpp23-adv-16.6.1" \
  test/checkers/automotive/cpp23/avoid-unused-variadic-template.cpp \
  --
```

### Run on Example
```bash
./build/bin/clang-tidy \
  --checks="automotive-cpp23-adv-16.6.1" \
  examples/cpp23/avoid-unused-variadic-template-violation.cpp \
  --
```

## Integration

The check integrates with the existing clang-tidy automotive module:
- Follows naming conventions (Avoid* prefix)
- Uses `onEndOfTranslationUnit()` for whole-program analysis
- Compatible with LLVM 20 APIs
- Properly handles system headers (excluded from analysis)

## References
- MISRA C++:2023 Rule 16.6.1
- C++11/14/17/20/23 variadic templates
- LLVM 20 AST Matchers and Clang-Tidy API

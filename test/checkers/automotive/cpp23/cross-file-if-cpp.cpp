// RUN: %check_clang_tidy %s automotive-cpp23-req-19.1.2 %t -- -- -std=c++17
// Test for automotive-cpp23-req-19.1.2: #else/#elif/#endif shall be in same file as #if

// OK - all directives in same file
#ifdef FEATURE
  #define VALUE 1
#else
  #define VALUE 0
#endif

#if defined(DEBUG)
  constexpr int mode = 1;
#elif defined(RELEASE)
  constexpr int mode = 0;
#else
  constexpr int mode = -1;
#endif

// OK - nested conditionals in same file
#ifdef OUTER
  #ifdef INNER
    #define BOTH 1
  #endif
#endif

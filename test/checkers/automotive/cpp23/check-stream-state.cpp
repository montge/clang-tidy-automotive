// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-adv-30.0 %t

// Test MISRA C++:2023 Rule 30.0 (partial) - Standard library stream objects
// shall not be used while in a fail state

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void test_chained_operations() {
  int x, y, z;

  // Non-compliant - chained stream operations without state checking
  std::cin >> x >> y;
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  std::cin >> x >> y >> z;
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]
  // CHECK-MESSAGES: :[[@LINE-2]]:22: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  // Compliant - single operation
  std::cin >> x;

  // Non-compliant - single operation after previous operation without check
  std::cin >> x;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: stream operation without prior state check; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]
  if (std::cin.good()) {
    std::cin >> y;
  }
}

void test_output_chaining() {
  int x = 1, y = 2;

  // Non-compliant - chained output operations
  std::cout << x << y;
  // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  std::cout << "Value: " << x << std::endl;
  // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]
  // CHECK-MESSAGES: :[[@LINE-2]]:31: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  // Compliant - single operation
  std::cout << x;
}

void test_file_streams() {
  std::ifstream infile("test.txt");
  int value;

  // Non-compliant - chained operations
  infile >> value >> value;
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  // Compliant - single operation with check
  infile >> value;
  if (infile.good()) {
    infile >> value;
  }

  std::ofstream outfile("output.txt");

  // Non-compliant - chained output
  outfile << "Data: " << value;
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  // Compliant
  outfile << "Data: ";
  if (outfile.good()) {
    outfile << value;
  }
}

void test_string_streams() {
  std::stringstream ss;
  int x, y;

  // Non-compliant - chained operations
  ss >> x >> y;
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  // Compliant - single operation
  ss >> x;

  // Compliant - with state check
  ss >> x;
  if (!ss.fail()) {
    ss >> y;
  }
}

void test_getline() {
  std::string line;

  // Non-compliant - getline without checking result
  std::getline(std::cin, line);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: getline() result not checked; stream state shall be checked after use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  // Compliant - getline in condition
  if (std::getline(std::cin, line)) {
    // Use line
  }

  // Compliant - getline in while loop
  while (std::getline(std::cin, line)) {
    // Process line
  }
}

void test_state_checks() {
  int value;

  // Compliant - checking good()
  std::cin >> value;
  if (std::cin.good()) {
    // Process value
  }

  // Compliant - checking fail()
  std::cin >> value;
  if (std::cin.fail()) {
    // Handle error
  }

  // Compliant - checking eof()
  std::cin >> value;
  if (std::cin.eof()) {
    // Handle end of file
  }

  // Compliant - checking stream as bool
  std::cin >> value;
  if (std::cin) {
    // Process value
  }

  // Compliant - checking with negation
  std::cin >> value;
  if (!std::cin) {
    // Handle error
  }
}

void test_consecutive_operations() {
  int x, y;

  // Non-compliant - using stream multiple times without check in between
  std::cin >> x;
  std::cin >> y;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: stream operation without prior state check; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

  // Non-compliant - also flagged because no check after previous use
  std::cin >> x;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: stream operation without prior state check; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]
  if (std::cin.good()) {
    std::cin >> y;
  }
}

void test_mixed_streams() {
  // Compliant - different streams, no issue
  std::cin >> std::ws;
  std::cout << "Hello";

  // Each stream used independently
  int x;
  std::cin >> x;

  std::cout << x;
}

class StreamUser {
  std::ifstream file;

public:
  void read_data() {
    int value;

    // Non-compliant - chained operations
    file >> value >> value;
    // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: chained stream operations without state checking; stream state shall be checked before use (MISRA C++:2023 Rule 30.0) [automotive-cpp23-adv-30.0]

    // Compliant - with check
    file >> value;
    if (file.good()) {
      file >> value;
    }
  }
};

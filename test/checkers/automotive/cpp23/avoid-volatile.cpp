// RUN: %check_clang_tidy %s automotive-cpp23-req-10.4.1 %t
// Test for automotive-cpp23-req-10.4.1: The volatile keyword shall not be used

// Non-compliant: volatile outside allowed namespaces

// CHECK-MESSAGES: :[[@LINE+1]]:14: warning: the volatile keyword shall only be used for hardware access; volatile variable declared outside allowed namespaces
volatile int global_counter = 0;

// CHECK-MESSAGES: :[[@LINE+1]]:15: warning: the volatile keyword shall only be used for hardware access; volatile variable declared outside allowed namespaces
volatile bool flag = false;

class TestClass {
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: the volatile keyword shall only be used for hardware access; volatile field declared outside allowed namespaces
  volatile int member;

public:
  TestClass() : member(0) {}
};

namespace other {
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: the volatile keyword shall only be used for hardware access; volatile variable declared outside allowed namespaces
  volatile int counter = 0;
}

// Compliant: volatile in allowed namespaces

namespace hardware {
  // OK - in allowed namespace
  volatile unsigned int* device_register = nullptr;

  void write_register(volatile unsigned int* reg, unsigned int value) {
    *reg = value;
  }

  volatile unsigned int* get_register() {
    return device_register;
  }

  class DeviceController {
    volatile unsigned int* control_reg;
  public:
    DeviceController() : control_reg(nullptr) {}
  };
}

namespace hw {
  // OK - in allowed namespace (hw is in default allowed list)
  volatile unsigned char status_byte = 0;
}

namespace mmio {
  // OK - in allowed namespace (mmio is in default allowed list)
  struct MemoryMappedIO {
    volatile unsigned int* base_address;
  };
}

// Compliant: non-volatile usage
void compliant_examples() {
  // OK - no volatile
  int normal_var = 42;
  int* normal_ptr = &normal_var;
  (void)normal_ptr;
}

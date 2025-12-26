// RUN: %check_clang_tidy %s automotive-x-adv-2.4 %t -- -- -std=c11
// Test for automotive-x-adv-2.4: unused tag declarations should be avoided

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: unused tag 'UnusedStruct' [automotive-x-adv-2.4]
struct UnusedStruct {
  int x;
  int y;
};

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: unused tag 'UnusedEnum' [automotive-x-adv-2.4]
enum UnusedEnum {
  VALUE1,
  VALUE2
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: unused tag 'UnusedUnion' [automotive-x-adv-2.4]
union UnusedUnion {
  int i;
  float f;
};

// Note: The check doesn't track usage through local variable declarations,
// so these will also be flagged even though they appear to be used.

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: unused tag 'UsedStruct' [automotive-x-adv-2.4]
struct UsedStruct {
  int value;
};

void use_struct(void) {
  struct UsedStruct s;
  s.value = 10;
  (void)s;
}

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: unused tag 'UsedEnum' [automotive-x-adv-2.4]
enum UsedEnum {
  USED_VALUE1,
  USED_VALUE2
};

void use_enum(void) {
  enum UsedEnum e = USED_VALUE1;
  (void)e;
}

// Typedef of struct - the struct is used via typedef
typedef struct {
  int data;
} TypedefStruct;

void use_typedef(void) {
  TypedefStruct t;
  t.data = 5;
  (void)t;
}

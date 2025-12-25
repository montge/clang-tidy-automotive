// RUN: %check_clang_tidy %s automotive-c23-adv-2.4 %t -- -- -std=c11
// Test for automotive-c23-adv-2.4: unused tag declarations should be avoided

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: unused tag declaration 'UnusedStruct'
struct UnusedStruct {
  int x;
  int y;
};

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: unused tag declaration 'UnusedEnum'
enum UnusedEnum {
  VALUE1,
  VALUE2
};

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: unused tag declaration 'UnusedUnion'
union UnusedUnion {
  int i;
  float f;
};

// Used struct - should not trigger warning
struct UsedStruct {
  int value;
};

void use_struct(void) {
  struct UsedStruct s;
  s.value = 10;
  (void)s;
}

// Used enum - should not trigger warning
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

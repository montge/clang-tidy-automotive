#include <cstdio>

void test_uninitialized() {
  FILE *fp;  // Should warn
  fclose(fp);
}

void test_fopen_without_check() {
  FILE *fp = fopen("test.txt", "r");
  fread(nullptr, 1, 1, fp);  // Should warn about null check
  fclose(fp);  // Should warn about reassignment
}

void test_good_practice() {
  FILE *fp = fopen("test.txt", "r");
  if (fp == nullptr) {
    return;
  }
  char buffer[100];
  fread(buffer, 1, 100, fp);
  fclose(fp);
  fp = nullptr;
}

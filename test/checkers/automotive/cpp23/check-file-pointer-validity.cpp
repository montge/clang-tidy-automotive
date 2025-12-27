// RUN: %check_clang_tidy %s automotive-cpp23-adv-21.2 %t

#include <stdio.h>

void test_uninitialized_file_pointer() {
  FILE *fp;
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: FILE* variable 'fp' declared without initialization; using an uninitialized FILE* is undefined behavior [automotive-cpp23-adv-21.2]

  // Using uninitialized pointer
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
  fclose(fp); // Using uninitialized fp
}

void test_initialized_file_pointer() {
  FILE *fp = nullptr; // OK - initialized

  FILE *fp2 = fopen("test.txt", "r"); // OK - initialized with fopen
}

void test_static_file_pointer() {
  static FILE *fp; // OK - static variables are zero-initialized
}

void test_fopen_without_null_check_simple() {
  // Direct use without null check
  // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: FILE* returned from fopen may be NULL and should be checked before use [automotive-cpp23-adv-21.2]
  fread(nullptr, 1, 1, fopen("test.txt", "r"));
}

void test_fopen_with_assignment() {
  FILE *fp = fopen("test.txt", "r"); // OK - assigned to variable

  if (fp != nullptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    fclose(fp);
  }
}

void test_fopen_in_if_condition() {
  if (FILE *fp = fopen("test.txt", "r")) { // OK - checked in condition
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    fclose(fp);
  }
}

void test_fopen_with_ternary() {
  // First fopen is condition (checked), second fopen in true branch is NOT checked
  // CHECK-MESSAGES: :[[@LINE+1]]:39: warning: FILE* returned from fopen may be NULL and should be checked before use [automotive-cpp23-adv-21.2]
  FILE *fp = fopen("test.txt", "r") ? fopen("test.txt", "r") : nullptr;
  (void)fp;
}

void test_fclose_usage() {
  FILE *fp = fopen("test.txt", "r");
  if (fp) {
    fclose(fp);
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    // CHECK-FIXES: fclose(fp); fp = nullptr;

    // Using fp after close would be undefined behavior
    // (Full dataflow analysis would be needed to detect this)
  }
}

void test_fclose_with_reassignment() {
  FILE *fp = fopen("test.txt", "r");
  if (fp) {
    fclose(fp);
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    // CHECK-FIXES: fclose(fp); fp = nullptr;
    fp = nullptr; // Good practice
  }
}

void test_freopen() {
  FILE *fp = nullptr;
  fp = freopen("test.txt", "r", fp); // freopen also returns FILE*
}

void test_tmpfile() {
  FILE *fp = tmpfile(); // tmpfile also returns FILE*
  if (fp) {
    fclose(fp);
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    // CHECK-FIXES: fclose(fp); fp = nullptr;
  }
}

void test_file_operations() {
  FILE *fp = fopen("test.txt", "r");
  if (fp != nullptr) {
    char buffer[100];
    fread(buffer, 1, 100, fp);
    fwrite(buffer, 1, 100, fp);
    fgetc(fp);
    fputc('c', fp);
    fgets(buffer, 100, fp);
    fputs("test", fp);
    fprintf(fp, "%s", "test");
    fscanf(fp, "%s", buffer);
    fseek(fp, 0, 0);
    ftell(fp);
    rewind(fp);
    fflush(fp);
    feof(fp);
    ferror(fp);
    clearerr(fp);

    fclose(fp);
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    // CHECK-FIXES: fclose(fp); fp = nullptr;
  }
}

void test_double_close() {
  FILE *fp = fopen("test.txt", "r");
  if (fp) {
    fclose(fp);
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    // CHECK-FIXES: fclose(fp); fp = nullptr;

    // This would be a bug - double close
    // fclose(fp); // Undefined behavior (not detected by this simple check)
  }
}

// Compliant example
void compliant_file_handling() {
  FILE *fp = fopen("test.txt", "r");

  if (fp == nullptr) {
    // Handle error
    return;
  }

  // Use file
  char buffer[100];
  fread(buffer, 1, 100, fp);

  // Close file
  fclose(fp);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
  // CHECK-FIXES: fclose(fp); fp = nullptr;
  fp = nullptr; // Good practice

  // fp is now safely set to nullptr
}

class FileWrapper {
public:
  FileWrapper() : fp(nullptr) {} // OK - member initialized

private:
  FILE *fp; // OK - class member (not flagged as uninitialized local)
};

void test_parameter_file(FILE *fp) {
  // fp is a parameter, not flagged as uninitialized
  if (fp) {
    fclose(fp);
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: FILE* variable 'fp' closed here; ensure it is not used afterwards without being reassigned [automotive-cpp23-adv-21.2]
    // CHECK-FIXES: fclose(fp); fp = nullptr;
  }
}

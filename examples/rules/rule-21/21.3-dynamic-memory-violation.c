// MISRA C:2025 Rule 21.3 - Memory allocation functions shall not be used
// Category: Required
// Status: VIOLATION
//
// This code violates Rule 21.3 because it uses dynamic memory allocation
// functions (malloc, calloc, realloc, free). Dynamic memory can cause
// memory leaks, fragmentation, and unpredictable behavior in safety-critical
// systems.
//
// Expected diagnostic:
//   warning: use of dynamic memory allocation function 'malloc'
//   [automotive-avoid-dynamic-memory]

#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[32];
} Record;

Record* create_record(int id, const char* name) {
    Record* rec = (Record*)malloc(sizeof(Record));  // VIOLATION: malloc
    if (rec != NULL) {
        rec->id = id;
        strncpy(rec->name, name, sizeof(rec->name) - 1);
        rec->name[sizeof(rec->name) - 1] = '\0';
    }
    return rec;
}

void delete_record(Record* rec) {
    free(rec);  // VIOLATION: free
}

int main(void) {
    Record* r = create_record(1, "Test");
    if (r != NULL) {
        delete_record(r);
    }
    return 0;
}

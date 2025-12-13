// MISRA C:2025 Rule 21.3 - Memory allocation functions shall not be used
// Category: Required
// Status: COMPLIANT
//
// This code complies with Rule 21.3 by using static memory allocation.
// All memory is allocated at compile time with known, fixed sizes.

#include <string.h>
#include <stdbool.h>

#define MAX_RECORDS 100

typedef struct {
    int id;
    char name[32];
    bool in_use;
} Record;

// Static pool of records
static Record record_pool[MAX_RECORDS];
static bool pool_initialized = false;

static void init_pool(void) {
    if (!pool_initialized) {
        for (int i = 0; i < MAX_RECORDS; i++) {
            record_pool[i].in_use = false;
        }
        pool_initialized = true;
    }
}

Record* create_record(int id, const char* name) {
    init_pool();

    for (int i = 0; i < MAX_RECORDS; i++) {
        if (!record_pool[i].in_use) {
            record_pool[i].id = id;
            strncpy(record_pool[i].name, name, sizeof(record_pool[i].name) - 1);
            record_pool[i].name[sizeof(record_pool[i].name) - 1] = '\0';
            record_pool[i].in_use = true;
            return &record_pool[i];
        }
    }
    return NULL;  // Pool exhausted
}

void delete_record(Record* rec) {
    if (rec != NULL) {
        rec->in_use = false;
    }
}

int main(void) {
    Record* r = create_record(1, "Test");
    if (r != NULL) {
        delete_record(r);
    }
    return 0;
}

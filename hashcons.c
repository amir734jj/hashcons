#include <stdlib.h>
#include <string.h>
#include "prime.h"
#include "hashcons.h"

#define HC_INITIAL_BASE_SIZE 61
#define MAX_DENSITY 0.5

void hc_initialize(HASH_CONS_TABLE hc, const int capacity) {
    hc->capacity = capacity;
    hc->table = calloc(hc->capacity, sizeof(void *));
    hc->size = 0;
}

static int hc_candidate_index(HASH_CONS_TABLE hc, void *item, bool insert_mode) {
    int attempt = 0;
    int hash = hc->hashf(item);
    int index = hash % hc->capacity;
    int step_size = 0;

    while (attempt++ < hc->capacity) {
        if (insert_mode && hc->table[index] == NULL) {
            return index;
        } else if (!insert_mode && hc->equalf(hc->table[index], item)) {
            return index;
        }

        if (attempt == 0) {
            step_size = hash % (hc->capacity - 2);
        }
        index = (index + step_size) % hc->capacity;
    }

    return -1;
}

static void *hc_insert(HASH_CONS_TABLE hc, void *item) {
    int index = hc_candidate_index(hc, item, true);

    hc->table[index] = item;
    hc->size++;
}

static void *hc_search(HASH_CONS_TABLE hc, void *item) {
    int index = hc_candidate_index(hc, item, false);

    return index == -1 ? NULL : hc->table[index];
}

static void hc_resize(HASH_CONS_TABLE hc, const int capacity) {
    void **old_table = hc->table;
    int old_capacity = hc->capacity;
    hc_initialize(hc, capacity);

    for (int i = 0; i < old_capacity; i++) {
        void *item = old_table[i];
        if (item != NULL) {
            hc_insert(hc, item);
        }
    }

    free(old_table);
}

void *hash_cons_get(void *item, size_t temp_size, HASH_CONS_TABLE hc) {
    void *result;

    if (hc->table == NULL) {
        hc_initialize(hc, HC_INITIAL_BASE_SIZE);
    }

    if (hc->size > hc->capacity * MAX_DENSITY) {
        const int new_capacity = next_twin_prime((hc->capacity << 1) + 1);
        hc_resize(hc, new_capacity);
    }

    if ((result = hc_search(hc, item)) != NULL) {
        return result;
    } else {
        result = malloc(temp_size);
        memcpy(result, item, temp_size);

        hc_insert(hc, result);

        return result;
    }
}

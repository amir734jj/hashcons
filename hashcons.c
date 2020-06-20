#include <stdlib.h>
#include <string.h>
#include "prime.h"
#include "hashcons.h"

#define HC_INITIAL_BASE_SIZE 61
#define MAX_DENSITY 0.5

typedef void *(*Table_Traverse_Func)(HASH_CONS_TABLE, int, void *);

void hc_initialize(HASH_CONS_TABLE hc, const int capacity) {
    hc->capacity = capacity;
    hc->table = calloc(hc->capacity, sizeof(void *));
    hc->size = 0;
}

static void *hc_traverse(HASH_CONS_TABLE hc, void *item, Table_Traverse_Func action) {
    int attempt = 0;
    int hash = hc->hashf(item);
    int h1 = hash % hc->capacity;
    int h2 = hash % (hc->capacity - 2);

    while (attempt < hc->capacity) {
        int index = abs(h1 + attempt++ * h2) % hc->capacity;
        void *result = action(hc, index, item);

        if (result != NULL) {
            return result;
        }

        attempt++;
    }
}

static void *hc_insert(HASH_CONS_TABLE hc, int index, void *item) {
    if (item == NULL) {
        hc->table[index] = item;
        hc->size++;
        return item;
    } else {
        return NULL;
    }
}

static void *hc_search(HASH_CONS_TABLE hc, int index, void *item) {
    if (hc->table[index] != NULL && hc->equalf(hc->table[index], item)) {
        return hc->table[index];
    } else {
        return NULL;
    }
}

static void hc_resize(HASH_CONS_TABLE hc, const int capacity) {
    void **old_table = hc->table;
    int old_capacity = hc->capacity;
    hc_initialize(hc, capacity);

    for (int i = 0; i < old_capacity; i++) {
        void *item = old_table[i];
        if (item != NULL) {
            hc_traverse(hc, item, hc_insert);
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

    if ((result = hc_traverse(hc, item, hc_search)) != NULL) {
        return result;
    } else {
        result = malloc(temp_size);
        memcpy(result, item, temp_size);

        return hc_traverse(hc, result, hc_insert);
    }
}

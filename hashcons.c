#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "prime.h"
#include "hashcons.h"

#define HC_INITIAL_BASE_SIZE 61

// if it's bigger, we need to rehash
// if size > capacity * MAX_DENSITY then rehash
#define MAX_DENSITY 0.5

void hc_insert(HASH_CONS_TABLE hc, void *item);

void hc_initialize(HASH_CONS_TABLE hc, const int base_size) {
    hc->capacity = base_size;
//    hc->table = malloc(hc->capacity * sizeof(void *));
    hc->table = calloc(hc->capacity, sizeof(void *));
    hc->size = 0;
    int i;
    for (i = 0; i < hc->capacity; i++) {
        hc->table[i] = NULL;
    }
}


/**
 * Resizes the table by creating a temporary hash table for values to go off of.
 */
static void hc_resize(HASH_CONS_TABLE hc, const int capacity) {

    printf("start resized from %d to %d\n", hc->capacity, capacity);

    HASH_CONS_TABLE temp_hc = malloc(sizeof(struct hash_cons_table));
    hc_initialize(temp_hc, capacity);
    temp_hc->equalf = hc->equalf;
    temp_hc->hashf = hc->hashf;

    for (int i = 0; i < hc->size; i++) {
        void *item = hc->table[i];
        if (item != NULL) {
            hc_insert(temp_hc, item);
        }
    }

    hc->table = temp_hc->table;
    hc->capacity = capacity;
    free(temp_hc);

    printf("end resized from %d to %d\n", hc->capacity, capacity);
}

/**
 * Increases the table size based on the "base size" by a factor of 2 + 1
 */
static void hc_resize_up(HASH_CONS_TABLE hc) {
    const int new_capacity = next_twin_prime((hc->capacity << 1) + 1);

    hc_resize(hc, new_capacity);
}

static int hc_get_index(const int index1, const int index2, const int attempt, const int capacity) {
    return (index1 + attempt * index2) % capacity;
}

static int hash1(HASH_CONS_TABLE hc, void *item) {
    return labs(hc->hashf(item)) % hc->capacity;
}

static int hash2(HASH_CONS_TABLE hc, void *item) {
    return labs(hc->hashf(item)) % (hc->capacity - 2);
}

/**
 * Inserts a key/value pair into the hash table.
 */
void hc_insert(HASH_CONS_TABLE hc, void *item) {
    if (hc->size > hc->capacity * MAX_DENSITY) {
        hc_resize_up(hc);
    }

    int h1 = hash1(hc, item);
    int h2 = hash2(hc, item);

    // if collision occurs
    if (hc->table[h1] != NULL) {
        int attempt = 1;
        while (TRUE) {
            // get new index
            int index = hc_get_index(h1, h2, attempt, hc->capacity);

            // if no collision occurs, store
            if (hc->table[index] == NULL) {
                hc->table[index] = item;
                break;
            }
            attempt++;
        }
    }
        // if no collision occurs
    else {
        hc->table[h1] = item;
    }

    hc->size++;
}

/**
 * Searches through the hash table for the value of the corresponding key. If nothing is found, NULL
 * is returned.
 */
void *hc_search(HASH_CONS_TABLE hc, void *item) {
    int h1 = hash1(hc, item);
    int h2 = hash2(hc, item);

    int attempt = 0;
    while (attempt < hc->capacity) {
        int index = hc_get_index(h1, h2, attempt, hc->capacity);

        // Failed to find
        if (hc->table[index] == NULL) {
            break;
        } else if (hc->equalf(hc->table[index], item)) {
            return hc->table[index];
        }

        attempt++;
    }

    return NULL;
}

void *hash_cons_get(void *item, size_t temp_size, HASH_CONS_TABLE hc) {
    // Initialize data-structure
    if (hc->table == NULL) {
        hc_initialize(hc, HC_INITIAL_BASE_SIZE);
    }

    void *search_result = hc_search(hc, item);

    if (search_result == NULL) {
        printf("did not find the item\n");
        // memcopy item before insert
        void *copied_item = malloc(temp_size);
        memcpy(copied_item, item, temp_size);

        hc_insert(hc, copied_item);

        return item;
    } else {
        printf("found the item\n");
        return search_result;
    }
}

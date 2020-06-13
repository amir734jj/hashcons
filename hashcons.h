#ifndef HASHCONS_H
#define HASHCONS_H

#include "include.h"

typedef long (*HASH_CONS_HASH)(void *);

typedef int (*HASH_CONS_EQUAL)(void *, void *);

typedef struct hash_cons_table {
    int size;
    int capacity;
    void **table;
    HASH_CONS_HASH hashf;
    HASH_CONS_EQUAL equalf;
} *HASH_CONS_TABLE;

/**
 * Get item if there is one otherwise create one
 * @param temp_item it is a temporary or perhaps stack allocated creation of item
 * @param temp_size how many bytes it is
 * @param table
 */
void *hash_cons_get(void *temp_item, size_t temp_size, HASH_CONS_TABLE table);

#endif
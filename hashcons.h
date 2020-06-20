#ifndef HASHCONS_H
#define HASHCONS_H

#include <stddef.h>
#include <stdbool.h>

typedef int (*Hash_Cons_Hash)(void *);
typedef bool (*Hash_Cons_Equal)(void *, void *);

typedef struct hash_cons_table {
    int size;
    int capacity;
    void **table;
    Hash_Cons_Hash hashf;
    Hash_Cons_Equal equalf;
} *HASH_CONS_TABLE;

/**
 * Get item if there is one otherwise create one
 * @param temp_item it is a temporary or perhaps stack allocated creation of item
 * @param temp_size how many bytes it is
 * @param hashcons table
 */
void *hash_cons_get(void *temp_item, size_t temp_size, HASH_CONS_TABLE table);

#endif
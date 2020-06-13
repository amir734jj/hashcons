#include "common.h"
#include "hashcons.h"
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

typedef struct dummy {
    int key;
} *DUMMY;

long hash(void *item) {
    return 13 * ((DUMMY) item)->key + 17;
}

int equal(void *item1, void *item2) {
    if (item1 == NULL || item2 == NULL) {
        return FALSE;
    }

    return ((DUMMY) item1)->key == ((DUMMY) item2)->key;
}

DUMMY create_dummy(int key) {
    DUMMY dummy = malloc(sizeof(dummy));
    dummy->key = key;
    return dummy;
}


int main() {
    HASH_CONS_TABLE hc = malloc(sizeof(struct hash_cons_table));
    hc->hashf = hash;
    hc->equalf = equal;
    hc->size = 0;
    int count = 300;

    printf("starting to add stuff\n");
    int i;
    for (i = 0; i < count; i++) {
        void *item = create_dummy(i);
        hash_cons_get(item, sizeof(struct dummy), hc);
    }
    printf("finished adding stuff\n");

    printf("starting to get stuff\n");
    for (i = 0; i < count; i++) {
        void *item = create_dummy(i);
        hash_cons_get(item, sizeof(struct dummy), hc);
    }
    printf("finished getting stuff\n");

    printf("Done!");

    test_integer_table();

    return 0;
}

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
        return false;
    }

    return ((DUMMY) item1)->key == ((DUMMY) item2)->key;
}

DUMMY create_dummy(int key) {
    DUMMY dummy = malloc(sizeof(dummy));
    dummy->key = key;
    return dummy;
}

static int test_adding_items(HASH_CONS_TABLE hc, int test_sample)
{
    printf("starting to add stuff\n");
    int failure_count = 0;
    for (int i = 0; i < test_sample; i++) {
        void *item = create_dummy(i);
        if (!hash_cons_get(item, sizeof(struct dummy), hc))
        {
            failure_count++;
        }
    }
    printf("finished adding stuff\n");

    return failure_count;
}

static int test_getting_times(HASH_CONS_TABLE hc, int test_sample)
{
    printf("starting to get stuff\n");
    int failure_count = 0;
    for (size_t i = 0; i < test_sample; i++) {
        void *item = create_dummy(i);

        if (hash_cons_get(item, sizeof(struct dummy), hc) == NULL)
        {
            failure_count++;
            printf("Item %d not found\n", i);
        }
    }
    printf("finished getting stuff\n");

    return failure_count;
}

int main() {
    HASH_CONS_TABLE hc = malloc(sizeof(*hc));
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
        if (hash_cons_get(item, sizeof(struct dummy), hc) == NULL)
        {
            printf("Item %d not found\n", i);
        }
    }
    printf("finished getting stuff\n");

    printf("Done!");

    test_integer_table();

    test_adding_items(hc, 100);

    test_getting_times(hc, 100);

    return 0;
}

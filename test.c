#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "hashcons.h"


long hash_integer(void *p) {
    return *((int *) p);
}


bool equals_integer(void *p1, void *p2) {
    int *i1 = (int *) p1;
    int *i2 = (int *) p2;
    return *i1 == *i2;
}

static struct hash_cons_table integer_table = {
        0, 0, 0,
        &hash_integer,
        &equals_integer
};


int *new_integer(int n) {
    return hash_cons_get(&n, sizeof(int), &integer_table);
}

void assertTrue(const char *message, bool b) {
    if (!b) {
        fprintf(stderr, "Assertion failed: %s\n", message);
        exit(1);
    }
}

void test_integer_table() {
    int *i3 = new_integer(3);
    assertTrue("initial i3", *i3 == 3);
    int *i8 = new_integer(8);
    assertTrue("initial i8", *i8 == 8);
    assertTrue("later i3", *i3 == 3);
    for (int i = 0; i < 100; ++i) {
        char buffer[256];
        sprintf(buffer, "integer for %d", i);
        assertTrue(buffer, *new_integer(i) == i);
    }
}
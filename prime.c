#include "prime.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INITIAL_TABLE_SIZE 9973

struct PrimesTable {
    int size;
    bool *table;
};

struct PrimesTable primesTable = {0, 0};

/**
 * Create a boolean array "prime[0..n]" and initialize
 * all entries it as true. A value in prime[i] will
 * finally be false if i is Not a prime, else true.
 */
void initialize_sieve_of_eratosthenes(int n) {
    if (primesTable.table == NULL) {
        primesTable.size = n;
        primesTable.table = malloc(n * sizeof(bool));
        memset(primesTable.table, true, primesTable.size);
    } else {
        int original_size = primesTable.size;
        bool *original_table = primesTable.table;

        primesTable.size = n;
        primesTable.table = malloc(n * sizeof(bool));
        memset(primesTable.table, true, primesTable.size);
        memcpy(primesTable.table, original_table, original_size * sizeof(bool));
        free(original_table);
    }

    for (int p = 2; p * p < n; p++) {
        // If primes[p] is not changed, then it is a prime
        if (primesTable.table[p] == true) {
            // Update all multiples of p
            for (int i = p * 2; i <= n; i += p) primesTable.table[i] = false;
        }
    }
}

/**
 * Return the next prime greater than parameter such that -2 is also a prime
 */
int next_twin_prime(int x) {
    if (primesTable.table == 0) {
        initialize_sieve_of_eratosthenes(INITIAL_TABLE_SIZE);
    }

    int i;
    for (i = x + 1; i < primesTable.size; i++) {
        if (primesTable.table[i] && primesTable.table[i - 2]) return i;
    }

    initialize_sieve_of_eratosthenes((primesTable.size << 1) + 1);

    return next_twin_prime(x);
}

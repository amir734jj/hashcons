#include "common.h"
#include <math.h>

/*
 * Returns whether x is prime or not.
 * 1 if prime
 * 0 if not prime
 * -1 if undefined.
 */
int is_prime(const int x)
{
    if (x < 2)
    {
        return -1;
    }
    if (x < 4)
    {
        return 1;
    }
    if ((x % 2) == 0)
    {
        return 0;
    }

    for (int i = 3; i <= floor(sqrt((double)x)); i += 2)
    {
        if ((x % i) == 0)
        {
            return 0;
        }
    }
    return 1;
}

/**
 * Returns next possible prime
 */
int next_prime(int x)
{
    while (is_prime(x) != 1)
    {
        x++;
    }

    return x;
}

/**
 * Return the next prime greater than parameter such that -2 is also a prime
 */
int next_twin_prime(int x)
{
    int attempts = 0;
    while (TRUE)
    {
        int prime = next_prime(x);
        if (is_prime(prime - 2))
        {
            return prime;
        }

        attempts++;
        x = prime + 1;
    }
}

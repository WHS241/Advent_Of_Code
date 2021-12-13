#ifndef DAY_25
#define DAY_25

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int day_25_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    size_t key1 = strtoul(argv[1], NULL, 10);
    size_t key2 = strtoul(argv[2], NULL, 10);

    size_t base = 7;
    size_t mod = 20201227;

    size_t secret1 = 0;
    size_t secret2 = 0;

    size_t i = 0;
    size_t curr = 1;
    while (!secret1 || !secret2) {
        if (curr == key1)
            secret1 = i;
        if (curr == key2)
            secret2 = i;

        curr = (curr * base) % mod;
        ++i;
    }

    size_t handshake = key2;
    for (size_t i = 1; i < secret1; ++i)
        handshake = (handshake * key2) % mod;

    printf("%zd\n", handshake);
    return 0;
}

#endif // DAY_25

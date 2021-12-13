#ifndef DAY_13
#define DAY_13

#include <stdio.h>

size_t gcd(size_t x, size_t y) {
    if (x > y)
        return gcd(y, x);
    if (x == 0)
        return 0;
    if (x == y || !(y % x))
        return x;
    return gcd(y % x, x);
}

size_t lcm(size_t x, size_t y) {
    return x * y / gcd(x, y);
}

int day_13_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    size_t start;
    size_t curr_id = 0;
    size_t curr_wait = 0;
    size_t curr_bus = 0;

    size_t part_2 = 0;
    size_t inc = 1;

    fscanf(reader, "%zd\n", &start);

    size_t i = 0;
    int c;
    while ((c = fgetc(reader)) != EOF) {
        if (c == ',' || c == '\n') {
            if (curr_id != 0) {
                size_t wait = curr_id - (start % curr_id);
                if (curr_wait == 0 || wait < curr_wait) {
                    curr_wait = wait;
                    curr_bus = curr_id;
                }

                while ((part_2 + i) % curr_id)
                    part_2 += inc;
                inc = lcm(inc, curr_id);
            }
            curr_id = 0;
            ++i;
        } else if (c != 'x') {
            curr_id = 10 * curr_id + (c - '0');
        }
    }

    printf("%zd\n%zd\n", curr_wait * curr_bus, part_2);

    return 0;
}

#endif // DAY_13

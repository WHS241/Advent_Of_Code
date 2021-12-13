#ifndef DAY_6
#define DAY_6

#include <stdio.h>

int day_6_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");
    const int NUM_LETTERS = 26;

    int answered[NUM_LETTERS];
    for (int i = 0; i < NUM_LETTERS; ++i)
        answered[i] = 0;

    int last_empty = 1;
    int num_counted = 1;

    int part1 = 0, part2 = 0;

    int c;
    while (1) {
        c = fgetc(reader);
        if (c == '\n' || c == EOF) {
            if (last_empty || c == EOF) {
                --num_counted;
                for (int i = 0; i < NUM_LETTERS; ++i) {
                    if (answered[i]) {
                        ++part1;
                        if (answered[i] == num_counted)
                            ++part2;
                    }
                    answered[i] = 0;
                }
                num_counted = 1;

                if (c == EOF)
                    break;
            } else {
                last_empty = 1;
                ++num_counted;
            }
            last_empty = 1;
        } else {
            last_empty = 0;
            ++answered[c - 'a'];
        }
    }

    printf("%d\n%d\n", part1, part2);

    return 0;
}

#endif // DAY_6

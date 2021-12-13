#ifndef DAY_2
#define DAY_2

#include <stdio.h>

#include "intcode.c"

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    const int TARGET = 19690720;

    for (int i = 0; i < 9999; ++i) {
        cell** tape = initialize(reader);
        set(tape, 1, i / 100);
        set(tape, 2, i % 100);
        run(tape);

        if (i == 1202) {
            printf("Part 1: %ld\n", get(tape, 0));
        }

        if (get(tape, 0) == TARGET) {
            printf("Part 2: %d\n", i);
            return 0;
        }
        clear_tape(tape);
        rewind(reader);
    }

    return 1;
}

#endif // DAY_2

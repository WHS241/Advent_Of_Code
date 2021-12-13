#ifndef DAY_5
#define DAY_5

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int day_5_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");
    int max_seat = 0;

    char buffer[11];
    while (fscanf(reader, "%s", buffer) != EOF) {
        int curr = 0;
        for (int i = 0; i < 10; ++i) {
            curr *= 2;
            if (buffer[i] == 'B' || buffer[i] == 'R')
                ++curr;
        }

        if (max_seat < curr)
            max_seat = curr;
    }

    printf("%d\n", max_seat);

    bool* contains = malloc((max_seat + 1) * sizeof(bool));

    for (int i = 0; i < max_seat; ++i)
        contains[i] = false;

    rewind(reader);

    while (fscanf(reader, "%s", buffer) != EOF) {
        int curr = 0;
        for (int i = 0; i < 10; ++i) {
            curr *= 2;
            if (buffer[i] == 'B' || buffer[i] == 'R')
                ++curr;
        }

        contains[curr] = true;
    }

    for (int i = 1; i < max_seat; ++i)
        if (!contains[i] && contains[i - 1] && contains[i + 1])
            printf("%d\n", i);

    free(contains);

    return 0;
}

#endif // DAY_0

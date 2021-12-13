#ifndef DAY_3
#define DAY_3

#include <stdio.h>
#include <stdlib.h>

int day_3_main(int argc, char** argv) {
    if (argc < 4)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    int dy = atoi(argv[2]), dx = atoi(argv[3]);
    char map[1000][40];
    size_t num_rows = 0;

    while (fscanf(reader, "%s", map[num_rows]) != EOF)
        ++num_rows;

    size_t num_cols = 0;
    while (map[0][num_cols] != '\0')
        ++num_cols;

    size_t count = 0;

    int i = 0, j = 0;
    while (i < num_rows) {
        if (map[i][j] == '#')
            ++count;
        i += dx;
        j = (j + dy) % num_cols;
    }

    printf("%zd\n", count);

    return 0;
}

#endif // DAY_0

#ifndef DAY_24
#define DAY_24

#include <stdio.h>
#include <stdlib.h>

int day_24_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    short grid[200][200];

    for (int i = 0; i < 200; ++i)
        for (int j = 0; j < 200; ++j)
            grid[i][j] = 0;

    int curr_x = 100;
    int curr_y = 100;
    int dir;

    while ((dir = fgetc(reader)) != EOF) {
        switch (dir) {
        case '\n':
            grid[curr_x][curr_y] = 1 - grid[curr_x][curr_y];
            curr_x = curr_y = 100;
            break;

        case 'e':
            ++curr_x;
            break;

        case 'w':
            --curr_x;
            break;

        case 'n':
            ++curr_y;
            if (fgetc(reader) == 'w')
                --curr_x;
            break;

        case 's':
            --curr_y;
            if (fgetc(reader) == 'e')
                ++curr_x;
            break;

        default:
            break;
        }
    }

    size_t count_black = 0;
    for (int i = 0; i < 200; ++i)
        for (int j = 0; j < 200; ++j)
            count_black += grid[i][j];

    printf("%zd\n", count_black);

    size_t num_iter = argc > 2 ? strtoul(argv[2], NULL, 10) : 100;
    for (size_t i = 0; i < num_iter; ++i) {
        short temp[200][200];
        for (int j = 0; j < 200; ++j)
            for (int k = 0; k < 200; ++k)
                temp[j][k] = 0;

        for (int j = 0; j < 200; ++j) {
            for (int k = 0; k < 200; ++k) {
                if (grid[j][k]) {
                    if (j != 0) {
                        ++temp[j - 1][k];
                        if (k != 199) {
                            ++temp[j - 1][k + 1];
                        }
                    }
                    if (j != 199) {
                        ++temp[j + 1][k];
                        if (k != 0) {
                            ++temp[j + 1][k - 1];
                        }
                    }
                    if (k != 0)
                        ++temp[j][k - 1];
                    if (k != 199)
                        ++temp[j][k + 1];
                }
            }
        }

        for (int j = 0; j < 200; ++j)
            for (int k = 0; k < 200; ++k)
                grid[j][k] = temp[j][k] == 2 || (grid[j][k] && temp[j][k] == 1);
    }

    count_black = 0;
    for (int i = 0; i < 200; ++i)
        for (int j = 0; j < 200; ++j)
            count_black += grid[i][j];

    printf("%zd\n", count_black);

    return 0;
}

#endif // DAY_24

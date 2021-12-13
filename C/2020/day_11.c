#ifndef DAY_11
#define DAY_11 2

#include <stdio.h>
#include <stdlib.h>

#if DAY_11 == 1
const int THRESHOLD = 4;
#elif DAY_11 == 2
const int THRESHOLD = 5;
#endif

int day_11_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    size_t rows = 0;
    size_t cols = 0;

    const char FLOOR = '.';
    const char EMPTY = 'L';
    const char FILLD = '#';

    int c;
    while ((c = fgetc(reader)) != EOF)
        if (c == '\n')
            ++rows;
        else if (!rows)
            ++cols;

    char** grid = malloc(sizeof(char*) * rows);
    for (size_t i = 0; i < rows; ++i)
        grid[i] = malloc(sizeof(char) * cols);

    rewind(reader);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j)
            grid[i][j] = fgetc(reader);
        fgetc(reader);
    }

    int changed = 1;
    while (changed) {
        changed = 0;

        char** temp = malloc(sizeof(char*) * rows);
        for (size_t i = 0; i < rows; ++i)
            temp[i] = malloc(sizeof(char) * cols);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] != FLOOR) {
                    int dirs[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                      {0, 1},   {1, -1}, {1, 0},  {1, 1}};
                    int count_neighbors = 0;
                    for (int k = 0; k < 8; ++k) {
                        int i2 = i + dirs[k][0];
                        int j2 = j + dirs[k][1];
#if DAY_11 == 2
                        while (i2 >= 0 && i2 < rows && j2 >= 0 && j2 < cols
                               && grid[i2][j2] == FLOOR) {
                            i2 += dirs[k][0];
                            j2 += dirs[k][1];
                        }
#endif
                        if (i2 >= 0 && i2 < rows && j2 >= 0 && j2 < cols && grid[i2][j2] == FILLD)
                            ++count_neighbors;
                    }

                    temp[i][j] = ((grid[i][j] == EMPTY && !count_neighbors)
                                  || (grid[i][j] == FILLD && count_neighbors < THRESHOLD))
                                 ? FILLD
                                 : EMPTY;
                    changed = changed || temp[i][j] - grid[i][j];
                } else {
                    temp[i][j] = grid[i][j];
                }
            }
        }

        void* swap = temp;
        temp = grid;
        grid = swap;

        for (size_t i = 0; i < rows; ++i)
            free(temp[i]);
        free(temp);
    }

    int count = 0;
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            if (grid[i][j] == FILLD)
                ++count;

    printf("%d\n", count);

    for (size_t i = 0; i < rows; ++i)
        free(grid[i]);
    free(grid);

    return 0;
}

#endif // DAY_11

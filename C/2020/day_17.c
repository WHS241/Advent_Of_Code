#ifndef DAY_17
#define DAY_17

#include <stdio.h>

#define DIM    8
#define NUM_IT 6

int day_17_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    const int one_layer_start = 1 + 2 * NUM_IT;
    const int read_start = DIM + 2 * NUM_IT;

    char part_1[2][one_layer_start][read_start][read_start];
    char part_2[2][one_layer_start][one_layer_start][read_start][read_start];

    const char inactive = '.';
    const char active = '#';

    for (int t = 0; t < 2; ++t) {
        for (int x = 0; x < read_start; ++x) {
            for (int y = 0; y < read_start; ++y) {
                for (int z = 0; z < one_layer_start; ++z) {
                    part_1[t][z][x][y] = inactive;
                    for (int w = 0; w < one_layer_start; ++w)
                        part_2[t][w][z][x][y] = inactive;
                }
            }
        }
    }

    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j)
            part_1[0][NUM_IT][NUM_IT + i][NUM_IT + j] =
              part_2[0][NUM_IT][NUM_IT][NUM_IT + i][NUM_IT + j] = fgetc(reader);
        fgetc(reader);
    }

    for (int t = 0; t <= NUM_IT; ++t) {
        for (int i = 0; i < one_layer_start; ++i) {
            for (int j = 0; j < read_start; ++j) {
                for (int k = 0; k < read_start; ++k) {
                    char neighbors[3][3][3];
                    for (int a = 0; a < 3; ++a)
                        for (int b = 0; b < 3; ++b)
                            for (int c = 0; c < 3; ++c)
                                neighbors[a][b][c] =
                                  (i + a - 1 >= 0 && i + a - 1 < one_layer_start && j + b - 1 >= 0
                                   && j + b - 1 < read_start && k + c - 1 >= 0
                                   && k + c - 1 < read_start)
                                    ? part_1[t % 2][i + a - 1][j + b - 1][k + c - 1]
                                    : inactive;
                    int num_neighbors = 0;
                    for (int a = 0; a < 3; ++a)
                        for (int b = 0; b < 3; ++b)
                            for (int c = 0; c < 3; ++c)
                                if (!(a == 1 && b == 1 && c == 1) && neighbors[a][b][c] == active)
                                    ++num_neighbors;

                    part_1[(t + 1) % 2][i][j][k] =
                      num_neighbors == 3 || (num_neighbors == 2 && part_1[t % 2][i][j][k] == active)
                        ? active
                        : inactive;
                }
            }
        }
    }

    int num_active = 0;
    for (int i = 0; i < one_layer_start; ++i)
        for (int j = 0; j < read_start; ++j)
            for (int k = 0; k < read_start; ++k)
                if (part_1[NUM_IT % 2][i][j][k] == active)
                    ++num_active;
    printf("%d\n", num_active);

    for (int t = 0; t <= NUM_IT; ++t) {
        for (int h = 0; h < one_layer_start; ++h) {
            for (int i = 0; i < one_layer_start; ++i) {
                for (int j = 0; j < read_start; ++j) {
                    for (int k = 0; k < read_start; ++k) {
                        char neighbors[3][3][3][3];
                        for (int a = 0; a < 3; ++a)
                            for (int b = 0; b < 3; ++b)
                                for (int c = 0; c < 3; ++c)
                                    for (int d = 0; d < 3; ++d)
                                        neighbors[a][b][c][d] =
                                          (h + a - 1 >= 0 && h + a - 1 < one_layer_start
                                           && i + b - 1 >= 0 && i + b - 1 < one_layer_start
                                           && j + c - 1 >= 0 && j + c - 1 < read_start
                                           && k + d - 1 >= 0 && k + d - 1 < read_start)
                                            ? part_2[t % 2][h + a - 1][i + b - 1][j + c - 1]
                                                    [k + d - 1]
                                            : inactive;
                        int num_neighbors = 0;
                        for (int a = 0; a < 3; ++a)
                            for (int b = 0; b < 3; ++b)
                                for (int c = 0; c < 3; ++c)
                                    for (int d = 0; d < 3; ++d)
                                        if (!(a == 1 && b == 1 && c == 1 && d == 1)
                                            && neighbors[a][b][c][d] == active)
                                            ++num_neighbors;

                        part_2[(t + 1) % 2][h][i][j][k] =
                          num_neighbors == 3
                              || (num_neighbors == 2 && part_2[t % 2][h][i][j][k] == active)
                            ? active
                            : inactive;
                    }
                }
            }
        }
    }

    num_active = 0;
    for (int h = 0; h < one_layer_start; ++h)
        for (int i = 0; i < one_layer_start; ++i)
            for (int j = 0; j < read_start; ++j)
                for (int k = 0; k < read_start; ++k)
                    if (part_2[NUM_IT % 2][h][i][j][k] == active)
                        ++num_active;
    printf("%d\n", num_active);

    return 0;
}

#endif // DAY_17

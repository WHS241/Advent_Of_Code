#ifndef DAY_10
#define DAY_10

#include <stdio.h>
#include <stdlib.h>

void sort(int* vals, int count) {
    for (int i = count - 1; i >= 0; --i) {
        int curr = i;
        while (2 * curr + 1 < count) {
            int target = 2 * curr + 1;
            if (target + 1 < count && vals[target + 1] > vals[target])
                ++target;
            if (vals[target] > vals[curr]) {
                int tmp = vals[curr];
                vals[curr] = vals[target];
                vals[target] = tmp;
                curr = target;
            } else {
                break;
            }
        }
    }

    for (int i = 0; i < count; ++i) {
        int tmp = vals[0];
        vals[0] = vals[count - i - 1];
        vals[count - i - 1] = tmp;

        int curr = 0;
        while (2 * curr + 1 < count - i - 1) {
            int target = 2 * curr + 1;
            if (target + 1 < count - i - 1 && vals[target + 1] > vals[target])
                ++target;
            if (vals[target] > vals[curr]) {
                int tmp = vals[curr];
                vals[curr] = vals[target];
                vals[target] = tmp;
                curr = target;
            } else {
                break;
            }
        }
    }
}

int day_10_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    int count = 0;
    while (fscanf(reader, "%*d") != EOF)
        ++count;

    rewind(reader);
    int* values = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        fscanf(reader, "%d", values + i);

    sort(values, count);

    int num_1 = 0;
    int num_3 = 0;

    // from 0
    switch (*values) {
    case 1:
        ++num_1;
        break;

    case 3:
        ++num_3;
        break;
    }

    for (int i = 1; i < count; ++i) {
        switch (values[i] - values[i - 1]) {
        case 1:
            ++num_1;
            break;

        case 3:
            ++num_3;
            break;

        default:
            break;
        }
    }
    ++num_3; // final jump

    printf("%d\n", num_1 * num_3);

    size_t* paths = malloc(sizeof(size_t) * count);
    for (int i = 0; i < count; ++i)
        paths[i] = 0;
    for (int i = 0; i < count; ++i) {
        if (values[i] <= 3)
            ++paths[i];
        for (int j = i + 1; j < count; ++j)
            if (values[j] - values[i] <= 3)
                paths[j] += paths[i];
            else
                break;
    }

    printf("%zd\n", paths[count - 1]);

    free(values);
    free(paths);
    return 0;
}

#endif // DAY_10

#ifndef DAY_2
#define DAY_2

#include <stdint.h>
#include <stdio.h>

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    char buffer[255];
    int i, j;
    char c;

    size_t count1 = 0, count2 = 0;

    while (fscanf(reader, "%d-%d %c: %s", &i, &j, &c, buffer) != EOF) {
        int c_count = 0;
        for (int k = 0; buffer[k] != '\0'; ++k)
            if (buffer[k] == c)
                ++c_count;
        if (c_count >= i && c_count <= j)
            ++count1;

        if ((buffer[i - 1] == c) ^ (buffer[j - 1] == c))
            ++count2;
    }

    printf("%zd\n%zd\n", count1, count2);

    return 0;
}

#endif // DAY_0

#ifndef DAY_1
#define DAY_1 2

#include <stdio.h>

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    int total = 0;
    int curr;

    while (fscanf(reader, "%d", &curr) != EOF) {
#if DAY_1 == 1
        total += curr / 3 - 2;
#elif DAY_1 == 2
        for (curr = curr / 3 - 2; curr > 0; curr = curr / 3 - 2)
            total += curr;
#endif
    }
    fclose(reader);

    printf("%d\n", total);

    return 0;
}

#endif // DAY_1

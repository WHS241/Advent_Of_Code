#ifndef DAY_1
#define DAY_1

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");
    
    int curr = 0;
    size_t pos = 0;
    bool hit = false;

    int c;
    while ((c = fgetc(reader)) != EOF) {
        if (!hit)
            ++pos;

        if (c == '(')
            ++curr;
        else if (c == ')') {
            --curr;
            if (curr < 0 && !hit)
                hit = true;
        }
    }    

    fclose(reader);
    
    printf("%d\n%zu\n", curr, pos);

    return 0;
}

#endif // DAY_0

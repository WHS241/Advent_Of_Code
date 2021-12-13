#ifndef DAY_8
#define DAY_8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char instr[4];
    int arg;
} op;

int run(op* ops, size_t num_lines, int change) {
    char tmp[4];

    if (change >= 0 && change < num_lines) {
        strcpy(tmp, ops[change].instr);
        if (!strcmp(tmp, "jmp"))
            strcpy(ops[change].instr, "nop");
        else if (!strcmp(tmp, "nop"))
            strcpy(ops[change].instr, "jmp");
    }

    short* visited = malloc(sizeof(short) * num_lines);
    for (size_t i = 0; i < num_lines; ++i)
        visited[i] = 0;

    int acc = 0;
    int i = 0;
    while (i >= 0 && i < num_lines) {
        if (visited[i])
            break;
        visited[i] = 1;
        if (!strcmp(ops[i].instr, "acc")) {
            acc += ops[i].arg;
            ++i;
        } else if (!strcmp(ops[i].instr, "nop")) {
            ++i;
        } else if (!strcmp(ops[i].instr, "jmp")) {
            i += ops[i].arg;
        }
    }

    free(visited);

    if (change < 0 || change >= num_lines) {
        return acc;
    }

    strcpy(ops[change].instr, tmp);
    return (i >= 0 && i < num_lines) ? 0 : acc;
}

int day_8_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    size_t num_lines = 0;

    for (int c = fgetc(reader); c != EOF; c = fgetc(reader))
        if (c == '\n')
            ++num_lines;
    rewind(reader);

    op* ops = malloc(sizeof(op) * num_lines);
    for (size_t i = 0; i < num_lines; ++i)
        fscanf(reader, "%s %d", ops[i].instr, &ops[i].arg);

    printf("%d\n", run(ops, num_lines, -1));

    for (size_t i = 0; i < num_lines; ++i) {
        int val = run(ops, num_lines, i);
        if (val)
            printf("%d\n", val);
    }

    free(ops);
    return 0;
}

#endif // DAY_8

#ifndef DAY_7
#define DAY_7

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[2][15];
} color;

typedef struct {
    int c;
    size_t amount;
    void* next;
} node;

void clear_list(node* n) {
    if (n != NULL)
        clear_list(n->next);
    free(n);
}

int day_7_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    int num_entries = 0;

    int c;
    while ((c = fgetc(reader)) != EOF) {
        if (c == '\n')
            ++num_entries;
    }

    rewind(reader);
    color* bags = malloc(sizeof(color) * num_entries);

    int i = 0;
    char buffer[2][15]; // color names only
    while (fscanf(reader, "%s %s", buffer[0], buffer[1]) != EOF) {
        strcpy(bags[i].name[0], buffer[0]);
        strcpy(bags[i].name[1], buffer[1]);
        ++i;
        for (c = fgetc(reader); c != EOF && c != '\n'; c = fgetc(reader)) {}
    }

    rewind(reader);
    i = 0;

    node** graph = malloc(sizeof(void*) * num_entries);
    for (int i = 0; i < num_entries; ++i)
        graph[i] = NULL;

    while (fscanf(reader, "%*s %*s %*s %*s") != EOF) {
        char num_buf[5];
        fscanf(reader, "%s %s %s", num_buf, buffer[0], buffer[1]);
        if (isdigit(num_buf[0])) {
            int progress = 1;

            while (progress) {
                int count = atoi(num_buf);
                int j = 0;
                while (
                  j < num_entries
                  && (strcmp(buffer[0], bags[j].name[0]) || strcmp(buffer[1], bags[j].name[1])))
                    ++j;
                if (j == num_entries) {
                    puts("ERROR: cannot find color\n");
                    exit(1);
                }

                node* edge = malloc(sizeof(node));
                edge->next = graph[i];
                graph[i] = edge;
                edge->c = j;
                edge->amount = count;

                fscanf(reader, "%s", buffer[0]);
                progress = buffer[0][3] == ',' || buffer[0][4] == ',';

                if (progress)
                    fscanf(reader, "%s %s %s", num_buf, buffer[0], buffer[1]);
            }
        }
        ++i;
    }

    i = 0;
    int* top_order = malloc(sizeof(int) * num_entries);
    size_t* vert_info = malloc(sizeof(size_t) * num_entries);
    for (int i = 0; i < num_entries; ++i)
        vert_info[i] = 0;
    for (int i = 0; i < num_entries; ++i)
        for (node* ptr = graph[i]; ptr != NULL; ptr = ptr->next)
            ++vert_info[ptr->c];
    for (int j = 0; j < num_entries; ++j)
        if (!vert_info[j])
            top_order[i++] = j;

    for (int j = 0; j < num_entries; ++j) {
        for (node* ptr = graph[top_order[j]]; ptr != NULL; ptr = ptr->next) {
            --vert_info[ptr->c];
            if (!vert_info[ptr->c])
                top_order[i++] = ptr->c;
        }
    }

    int target = 0;
    while (target < num_entries
           && (strcmp(bags[top_order[target]].name[0], "shiny")
               || strcmp(bags[top_order[target]].name[1], "gold")))
        ++target;

    if (target == num_entries) {
        puts("ERROR: no shiny gold\n");
        exit(1);
    }

    vert_info[top_order[target]] = 1;
    for (int i = target; i > 0; --i) {
        int curr = top_order[i - 1];
        for (node* ptr = graph[curr]; ptr != NULL; ptr = ptr->next) {
            if (vert_info[ptr->c]) {
                vert_info[curr] = 1;
                break;
            }
        }
    }

    int part1 = 0;
    for (int i = 0; i < target; ++i)
        if (vert_info[top_order[i]])
            ++part1;
    printf("%d\n", part1);

    for (int i = target; i < num_entries; ++i)
        for (node* ptr = graph[top_order[i]]; ptr != NULL; ptr = ptr->next)
            vert_info[ptr->c] += ptr->amount * vert_info[top_order[i]];

    size_t part2 = 0;
    for (int i = target + 1; i < num_entries; ++i)
        part2 += vert_info[top_order[i]];

    printf("%zd\n", part2);

    free(bags);
    for (int i = 0; i < num_entries; ++i)
        clear_list(graph[i]);
    free(graph);
    free(top_order);
    free(vert_info);

    return 0;
}

#endif // DAY_7

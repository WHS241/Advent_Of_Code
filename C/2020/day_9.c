#ifndef DAY_9
#define DAY_9

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int val;
    void* next;
} node;

int day_9_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    int avail[25];
    int i = 0;

    int curr;
    int target;
    while (fscanf(reader, "%d", &curr) != EOF) {
        if (i >= 25) {
            int found = 0;
            for (int j = 0; !found && j < 25; ++j)
                for (int k = j + 1; !found && k < 25; ++k)
                    if (avail[j] + avail[k] == curr)
                        found = avail[j];
            if (!found) {
                target = curr;
                break;
            }
        }

        avail[(i++ % 25)] = curr;
    }

    printf("%d\n", target);

    rewind(reader);
    node* start = NULL;
    node* end = NULL;
    int total = 0;

    while (total != target) {
        int curr;
        fscanf(reader, "%d", &curr);
        if (end) {
            end->next = malloc(sizeof(node));
            end = end->next;
        } else {
            start = end = malloc(sizeof(node));
        }
        *end = (node){.next = NULL, .val = curr};
        total += curr;
        while (total > target) {
            node* tmp = start;
            total -= tmp->val;

            start = start->next;
            free(tmp);

            if (!start)
                end = NULL;
        }
    }

    printf("%d\n", start->val + end->val);

    while (start) {
        node* tmp = start;
        start = start->next;
        free(tmp);
    }

    return 0;
}

#endif // DAY_9

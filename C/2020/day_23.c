#ifndef DAY_23
#define DAY_23

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t val;
    void* prev;
    void* next;
} cup;

int day_23_main(int argc, char** argv) {
    if (argc < 4)
        return 1;

    char* initial = argv[1];
    size_t num_cups = strtoul(argv[2], NULL, 10);
    size_t num_moves = strtoul(argv[3], NULL, 10);

    cup* curr = malloc(sizeof(cup));
    curr->prev = curr->next = curr;
    curr->val = initial[0] - '0';

    cup* runner = curr;
    for (size_t i = 1; i < num_cups; ++i) {
        runner->next = malloc(sizeof(cup));
        ((cup*) runner->next)->prev = runner;
        runner = runner->next;
        runner->next = curr;
        curr->prev = runner;
        runner->val = i < strlen(initial) ? initial[i] - '0' : i + 1;
    }

    cup** trackers = malloc(num_cups * sizeof(cup*));
    trackers[curr->val - 1] = curr;
    for (runner = curr->next; runner != curr; runner = runner->next)
        trackers[runner->val - 1] = runner;

    for (size_t i = 0; i < num_moves; ++i) {
        cup* removed_head = curr->next;
        cup* removed_mid = removed_head->next;
        cup* removed_tail = removed_mid->next;
        curr->next = removed_tail->next;
        ((cup*) removed_tail->next)->prev = curr;

        size_t target = curr->val;
        bool removed = true;
        while (removed) {
            --target;
            if (target == 0)
                target = num_cups;
            removed = removed_head->val == target || removed_mid->val == target
                   || removed_tail->val == target;
        }

        cup* target_cup = trackers[target - 1];
        removed_head->prev = target_cup;
        removed_tail->next = target_cup->next;
        ((cup*) removed_tail->next)->prev = removed_tail;
        target_cup->next = removed_head;

        curr = curr->next;
    }

    curr = trackers[0];
    if (num_cups <= strlen(initial)) {
        for (size_t i = 1; i < num_cups; ++i) {
            curr = curr->next;
            putchar('0' + curr->val);
        }
        putchar('\n');
    } else {
        size_t prod = 1;
        for (int i = 0; i < 2; ++i) {
            curr = curr->next;
            prod *= curr->val;
        }
        printf("%zd\n", prod);
    }

    return 0;
}

#endif // DAY_23

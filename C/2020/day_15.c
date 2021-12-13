#ifndef DAY_15
#define DAY_15

#include <stdio.h>
#include <stdlib.h>

const int BUCKETS = 65536;

typedef struct {
    int key;
    int val;
    void* next;
} node;

void cleanup(node* ptr) {
    if (ptr)
        cleanup(ptr->next);
    free(ptr);
}

int day_15_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    int num_iterations = atoi(argv[1]);

    node* map[BUCKETS];
    for (int i = 0; i < BUCKETS; ++i)
        map[i] = NULL;

    int curr;
    int next;
    for (int i = 1; i <= num_iterations; ++i) {
        curr = (i <= argc - 2) ? atoi(argv[i + 1]) : next;
        int last = i;

        node* ptr = map[curr % BUCKETS];
        while (ptr && ptr->key != curr)
            ptr = ptr->next;
        if (ptr) {
            last = ptr->val;
            ptr->val = i;
        } else {
            node* temp = malloc(sizeof(node));
            temp->key = curr;
            temp->val = i;
            temp->next = map[curr % BUCKETS];
            map[curr % BUCKETS] = temp;
        }

        next = i - last;
    }
    for (int i = 0; i < BUCKETS; ++i)
        cleanup(map[i]);

    printf("%d\n", curr);

    return 0;
}

#endif // DAY_15

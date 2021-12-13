#ifndef DAY_1
#define DAY_1 2

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v;
    void* next;
} node;

void clear_nodes(void* head) {
    if (head != NULL) {
        clear_nodes(((node*) head)->next);
        free(head);
    }
}

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");
    void* head = NULL;

    int ret, curr;
    const int TARGET = 2020;

    while ((ret = fscanf(reader, "%d\n", &curr)) != EOF) {
#if DAY_1 == 1
        for (node* p = head; p != NULL && p->v >= TARGET - curr; p = p->next) {
            if (p->v == TARGET - curr) {
                printf("%d\n", curr * p->v);
                clear_nodes(head);
                return 0;
            }
        }
#elif DAY_1 == 2
        for (node* p = head; p != NULL; p = p->next) {
            int rem = TARGET - curr - p->v;
            if (rem > 0) {
                for (node* p2 = head; p2 != NULL && p2->v >= rem; p2 = p2->next) {
                    if (p2->v == rem) {
                        printf("%d\n", curr * p->v * p2->v);
                        clear_nodes(head);
                        return 0;
                    }
                }
            }
        }
#endif

        node* ins = malloc(sizeof(node));
        ins->v = curr;

        if (head == NULL) {
            ins->next = NULL;
            head = ins;
        } else {
            node* p = head;
            while (p->next != NULL) {
                if (((node*) (p->next))->v < curr)
                    break;
                p = p->next;
            }
            if (p->v >= curr) {
                ins->next = p->next;
                p->next = ins;
            } else {
                ins->next = p;
                head = ins;
            }
        }
    }

    return 0;
}

#endif // DAY_0

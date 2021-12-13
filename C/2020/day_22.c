#ifndef DAY_22
#define DAY_22 2

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t val;
    void* next;
} list;

typedef struct {
    char pos[100];
    void* next;
} history;

void delete_list(list* head) {
    while (head) {
        list* temp = head;
        head = head->next;
        free(temp);
    }
}

void clear_history(history* head) {
    while (head) {
        history* temp = head;
        head = head->next;
        free(temp);
    }
}

// 输出：是不是P1赢了
bool play_deck(list** deck1, list** deck2) {
    if (!*deck1 || !*deck2)
        return *deck1;
    list* tail1 = *deck1;
    list* tail2 = *deck2;

    size_t size1 = 1;
    size_t size2 = 1;

    while (tail1->next) {
        tail1 = tail1->next;
        ++size1;
    }
    while (tail2->next) {
        tail2 = tail2->next;
        ++size2;
    }

    history* positions = NULL;
    while (*deck1 && *deck2) {
        // 有没有重复
        history* curr = malloc(sizeof(history));
        curr->pos[size1] = ' ';
        curr->pos[size1 + size2 + 1] = '\0';
        list* runner = *deck1;
        size_t i = 0;
        while (runner) {
            curr->pos[i++] = ' ' + runner->val;
            runner = runner->next;
        }
        i = 0;
        runner = *deck2;
        while (runner) {
            curr->pos[size1 + ++i] = ' ' + runner->val;
            runner = runner->next;
        }

        history* hist_runner = positions;
        while (hist_runner) {
            if (strcmp(hist_runner->pos, curr->pos) == 0) {
                clear_history(positions);
                free(curr);
                return true;
            }
            hist_runner = hist_runner->next;
        }
        curr->next = positions;
        positions = curr;

        bool p1_wins;
        if ((*deck1)->val < size1 && (*deck2)->val < size2) {
            list* subdeck1 = malloc(sizeof(list));
            list* subdeck2 = malloc(sizeof(list));
            list* subdeck_runner = subdeck1;

            list* deck_runner = (*deck1)->next;
            subdeck1->val = deck_runner->val;
            subdeck1->next = NULL;
            deck_runner = deck_runner->next;
            for (size_t i = 1; i < (*deck1)->val; ++i) {
                subdeck_runner->next = malloc(sizeof(list));
                subdeck_runner = subdeck_runner->next;
                subdeck_runner->val = deck_runner->val;
                subdeck_runner->next = NULL;
                deck_runner = deck_runner->next;
            }

            deck_runner = (*deck2)->next;
            subdeck2->val = deck_runner->val;
            subdeck2->next = NULL;
            deck_runner = deck_runner->next;
            subdeck_runner = subdeck2;
            for (size_t i = 1; i < (*deck2)->val; ++i) {
                subdeck_runner->next = malloc(sizeof(list));
                subdeck_runner = subdeck_runner->next;
                subdeck_runner->val = deck_runner->val;
                subdeck_runner->next = NULL;
                deck_runner = deck_runner->next;
            }

            p1_wins = play_deck(&subdeck1, &subdeck2);
            delete_list(subdeck1);
            delete_list(subdeck2);
        } else {
            p1_wins = (*deck1)->val > (*deck2)->val;
        }

        if (p1_wins) {
            ++size1;
            --size2;
            tail1->next = *deck1;
            *deck1 = (*deck1)->next;
            tail1 = ((list*) tail1->next)->next = *deck2;
            *deck2 = (*deck2)->next;
            tail1->next = NULL;
        } else {
            --size1;
            ++size2;
            tail2->next = *deck2;
            *deck2 = (*deck2)->next;
            tail2 = ((list*) tail2->next)->next = *deck1;
            *deck1 = (*deck1)->next;
            tail2->next = NULL;
        }
    }

    return *deck1 != NULL;
}

int day_22_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    char word[20];
    while (fgetc(reader) != '\n')
        ;

    list* deck1 = NULL;
    list* deck2 = NULL;
    list* curr1 = NULL;
    list* curr2 = NULL;

    bool on_first = true;
    size_t num_cards = 0;

    while (fscanf(reader, "%s ", word) != EOF) {
        if (isalpha(word[0])) {
            while (fgetc(reader) != '\n')
                ;
            on_first = false;
        } else {
            ++num_cards;
            size_t curr_val = strtoul(word, NULL, 10);
            list* curr = malloc(sizeof(list));
            curr->val = curr_val;
            curr->next = NULL;
            if (on_first) {
                if (deck1)
                    curr1->next = curr;
                else
                    deck1 = curr;
                curr1 = curr;
            } else {
                if (deck2)
                    curr2->next = curr;
                else
                    deck2 = curr;
                curr2 = curr;
            }
        }
    }

#if DAY_22 == 1
    while (deck1 && deck2) {
        if (deck1->val > deck2->val) {
            curr1->next = deck1;
            deck1 = deck1->next;
            curr1 = ((list*) curr1->next)->next = deck2;
            deck2 = deck2->next;
            curr1->next = NULL;
        } else {
            curr2->next = deck2;
            deck2 = deck2->next;
            curr2 = ((list*) curr2->next)->next = deck1;
            deck1 = deck1->next;
            curr2->next = NULL;
        }
    }
#elif DAY_22 == 2
    play_deck(&deck1, &deck2);
#endif

    if (!deck1)
        deck1 = deck2;
    curr2 = deck1;
    size_t total = 0;
    for (size_t i = 0; i < num_cards; ++i) {
        total += (num_cards - i) * curr2->val;
        curr2 = curr2->next;
    }

    printf("%zd\n", total);

    return 0;
}

#endif // DAY_22

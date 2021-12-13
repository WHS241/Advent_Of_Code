#ifndef DAY_21
#define DAY_21

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char s[12];
    void* next;
} list_node;

typedef struct {
    char s[12];
} str_wrap;

typedef struct {
    char known[12];
    str_wrap* unknowns;
    size_t num_unknowns;
    bool resolved;
    void* next;
} translation;

typedef struct {
    char unknown[12];
    str_wrap* knowns;
    size_t num_knowns;
    bool resolved;
    void* next;
} rev_trans;

void delete_list(list_node* head) {
    while (head) {
        list_node* temp = head;
        head = head->next;
        free(temp);
    }
}

str_wrap* create_array(list_node* list, size_t size) {
    str_wrap* result = malloc(size * sizeof(str_wrap));
    list_node* curr = list;
    for (size_t i = 0; i < size; ++i) {
        strcpy(result[i].s, curr->s);
        curr = curr->next;
    }
    return result;
}

list_node* create_list(str_wrap* arr, size_t size) {
    list_node* curr = NULL;
    for (size_t i = 0; i < size; ++i) {
        list_node* temp = malloc(sizeof(list_node));
        temp->next = curr;
        strcpy(temp->s, arr[size - i - 1].s);
        curr = temp;
    }
    return curr;
}

void sort(str_wrap* arr, size_t size) {
    if (size <= 1 || !arr)
        return;
    size_t left = 1;
    size_t right = size - 1;
    while (left <= right) {
        while (left <= right && strcmp(arr[0].s, arr[left].s) >= 0)
            ++left;
        while (left <= right && strcmp(arr[0].s, arr[right].s) < 0)
            --right;
        if (left <= right) {
            str_wrap temp;
            strcpy(temp.s, arr[left].s);
            strcpy(arr[left].s, arr[right].s);
            strcpy(arr[right].s, temp.s);
            ++left;
            --right;
        }
    }
    str_wrap temp;
    strcpy(temp.s, arr->s);
    strcpy(arr->s, arr[right].s);
    strcpy(arr[right].s, temp.s);
    sort(arr, right);
    sort(arr + left, size - right - 1);
}

// return: target的新大小
size_t intersect(list_node** target, const list_node* src) {
    size_t size = 0;
    list_node* curr = *target;
    list_node* last = NULL;
    const list_node* curr_src = src;
    while (curr_src) {
        while (curr && strcmp(curr->s, curr_src->s) < 0) {
            list_node* temp = curr;
            curr = curr->next;
            if (*target == temp)
                *target = curr;
            else
                last->next = curr;
            free(temp);
        }
        if (curr && strcmp(curr->s, curr_src->s) == 0) {
            last = curr;
            curr = curr->next;
            ++size;
        }
        curr_src = curr_src->next;
    }
    return size;
}

int day_21_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    char curr[12];
    bool curr_is_unknown = true;
    list_node* curr_item_list = NULL;
    size_t curr_list_size = 0;

    translation* allergen_list = NULL;

    while (fscanf(reader, "%s ", curr) != EOF) {
        if (curr[0] == '(') {
            str_wrap* curr_item_arr = NULL;
            curr_is_unknown = false;
            curr_item_arr = create_array(curr_item_list, curr_list_size);
            delete_list(curr_item_list);
            sort(curr_item_arr, curr_list_size);
            curr_item_list = create_list(curr_item_arr, curr_list_size);
            free(curr_item_arr);

            // dedup
            for (list_node* curr_node = curr_item_list; curr_node && curr_node->next;
                 curr_node = curr_node->next) {
                if (strcmp(curr_node->s, ((list_node*) curr_node->next)->s) == 0) {
                    list_node* temp = curr_node->next;
                    curr_node->next = temp->next;
                    free(temp);
                }
            }
        } else if (curr_is_unknown) {
            list_node* curr_node = malloc(sizeof(list_node));
            curr_node->next = curr_item_list;
            curr_item_list = curr_node;
            strcpy(curr_item_list->s, curr);
            ++curr_list_size;
        } else {
            if (curr[strlen(curr) - 1] == ')')
                curr_is_unknown = true;
            curr[strlen(curr) - 1] = '\0';
            translation* found_cand = NULL;
            for (translation* curr_cand = allergen_list; !found_cand && curr_cand;
                 curr_cand = curr_cand->next) {
                if (strcmp(curr_cand->known, curr) == 0) {
                    found_cand = curr_cand;
                }
            }
            if (found_cand) {
                list_node* cand_list = create_list(found_cand->unknowns, found_cand->num_unknowns);
                free(found_cand->unknowns);
                found_cand->num_unknowns = intersect(&cand_list, curr_item_list);
                found_cand->unknowns = create_array(cand_list, found_cand->num_unknowns);
                delete_list(cand_list);
            } else {
                translation* new_node = malloc(sizeof(translation));
                new_node->next = allergen_list;
                allergen_list = new_node;
                new_node->num_unknowns = curr_list_size;
                new_node->unknowns = create_array(curr_item_list, curr_list_size);
                new_node->resolved = false;
                strcpy(new_node->known, curr);
            }

            if (curr_is_unknown) {
                delete_list(curr_item_list);
                curr_item_list = NULL;
                curr_list_size = 0;
            }
        }
    }

    size_t num_allergens = 0;
    for (translation* curr = allergen_list; curr; curr = curr->next)
        ++num_allergens;

    rev_trans* rev_look = NULL;
    for (translation* curr = allergen_list; curr; curr = curr->next) {
        for (size_t i = 0; i < curr->num_unknowns; ++i) {
            rev_trans* curr_entry = rev_look;
            while (curr_entry && strcmp(curr_entry->unknown, curr->unknowns[i].s))
                curr_entry = curr_entry->next;
            if (curr_entry) {
                strcpy(curr_entry->knowns[curr_entry->num_knowns++].s, curr->known);
            } else {
                rev_trans* temp = malloc(sizeof(rev_trans));
                temp->knowns = malloc(num_allergens * sizeof(str_wrap));
                strcpy(temp->knowns[0].s, curr->known);
                temp->num_knowns = 1;
                strcpy(temp->unknown, curr->unknowns[i].s);
                temp->next = rev_look;
                temp->resolved = false;
                rev_look = temp;
            }
        }
    }

    rewind(reader);
    size_t count_safe = 0;
    while (fscanf(reader, "%s ", curr) != EOF) {
        if (curr[0] == '(')
            while (fgetc(reader) != '\n')
                ;
        else {
            bool is_candidate = false;
            for (rev_trans* curr_cand = rev_look; !is_candidate && curr_cand;
                 curr_cand = curr_cand->next)
                if (strcmp(curr, curr_cand->unknown) == 0)
                    is_candidate = true;
            if (!is_candidate)
                ++count_safe;
        }
    }

    printf("%zd\n", count_safe);

    for (size_t i = 0; i < num_allergens; ++i) {
        char known[12];
        char unknown[12];
        known[0] = unknown[0] = '\0';
        for (rev_trans* curr = rev_look; curr; curr = curr->next) {
            if (!curr->resolved && curr->num_knowns == 1) {
                strcpy(unknown, curr->unknown);
                strcpy(known, curr->knowns[0].s);
                curr->resolved = true;
                break;
            }
        }

        if (strlen(known)) {
            for (translation* curr = allergen_list; curr; curr = curr->next) {
                if (strcmp(curr->known, known) == 0) {
                    free(curr->unknowns);
                    curr->num_unknowns = 1;
                    curr->unknowns = malloc(sizeof(str_wrap));
                    strcpy(curr->unknowns->s, unknown);
                    curr->resolved = true;
                    break;
                }
            }
            for (rev_trans* curr = rev_look; curr; curr = curr->next) {
                if (strcmp(curr->unknown, unknown) != 0) {
                    list_node* cand_list = create_list(curr->knowns, curr->num_knowns);
                    list_node* last = NULL;
                    for (list_node* curr_cand = cand_list; curr_cand;) {
                        if (strcmp(curr_cand->s, known) == 0) {
                            list_node* temp = curr_cand;
                            curr_cand = curr_cand->next;
                            if (temp == cand_list)
                                cand_list = curr_cand;
                            else
                                last->next = curr_cand;
                            free(temp);
                            --curr->num_knowns;
                        } else {
                            last = curr_cand;
                            curr_cand = curr_cand->next;
                        }
                    }
                    free(curr->knowns);
                    curr->knowns = create_array(cand_list, curr->num_knowns);
                    delete_list(cand_list);
                }
            }
        } else {
            for (translation* curr = allergen_list; curr; curr = curr->next) {
                if (!curr->resolved && curr->num_unknowns == 1) {
                    strcpy(known, curr->known);
                    strcpy(unknown, curr->unknowns[0].s);
                    curr->resolved = true;
                    break;
                }
            }
            for (rev_trans* curr = rev_look; curr; curr = curr->next) {
                if (strcmp(curr->unknown, unknown) == 0) {
                    free(curr->knowns);
                    curr->num_knowns = 1;
                    curr->knowns = malloc(sizeof(str_wrap));
                    strcpy(curr->knowns->s, known);
                    curr->resolved = true;
                    break;
                }
            }
            for (translation* curr = allergen_list; curr; curr = curr->next) {
                if (strcmp(curr->known, known) != 0) {
                    list_node* cand_list = create_list(curr->unknowns, curr->num_unknowns);
                    list_node* last = NULL;
                    for (list_node* curr_cand = cand_list; curr_cand;) {
                        if (strcmp(curr_cand->s, known) == 0) {
                            list_node* temp = curr_cand;
                            curr_cand = curr_cand->next;
                            if (temp == cand_list)
                                cand_list = curr_cand;
                            else
                                last->next = curr_cand;
                            free(temp);
                            --curr->num_unknowns;
                        } else {
                            last = curr_cand;
                            curr_cand = curr_cand->next;
                        }
                    }
                    free(curr->unknowns);
                    curr->unknowns = create_array(cand_list, curr->num_unknowns);
                    delete_list(cand_list);
                }
            }
        }
    }

    str_wrap* allergens = malloc(num_allergens * sizeof(str_wrap));
    size_t i = 0;
    for (translation* curr = allergen_list; curr; curr = curr->next)
        strcpy(allergens[i++].s, curr->known);

    sort(allergens, num_allergens);

    for (size_t i = 0; i < num_allergens; ++i) {
        if (i)
            putchar(',');
        for (translation* curr = allergen_list; curr; curr = curr->next)
            if (strcmp(allergens[i].s, curr->known) == 0) {
                printf("%s", curr->unknowns[0].s);
                break;
            }
    }

    putchar('\n');

    return 0;
}

#endif // DAY_21

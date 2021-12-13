#ifndef DAY_14
#define DAY_14

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t index;
    size_t val;
    void* next;
} node;

typedef struct {
    size_t val;
    void* next;
} val_node;

const size_t BUCKETS = 4096;
const size_t MASK_SIZE = 36;

void set_1(node** map, size_t index, size_t val) {
    size_t bucket_i = index % BUCKETS;
    node* ptr = map[bucket_i];
    if (ptr == NULL) {
        map[bucket_i] = malloc(sizeof(node));
        map[bucket_i]->index = index;
        map[bucket_i]->val = val;
        map[bucket_i]->next = NULL;
    } else {
        while (ptr->index != index && ptr->next != NULL)
            ptr = ptr->next;
        if (ptr->index != index) {
            ptr->next = malloc(sizeof(node));
            ptr = ptr->next;
            ptr->index = index;
            ptr->next = NULL;
        }
        ptr->val = val;
    }
}

void set_2(node** map, val_node* list, size_t val) {
    while (list) {
        size_t index = list->val;
        size_t bucket_i = index % BUCKETS;
        node* ptr = map[bucket_i];
        if (ptr == NULL) {
            map[bucket_i] = malloc(sizeof(node));
            map[bucket_i]->index = index;
            map[bucket_i]->val = val;
            map[bucket_i]->next = NULL;
        } else {
            while (ptr->index != index && ptr->next != NULL)
                ptr = ptr->next;
            if (ptr->index != index) {
                ptr->next = malloc(sizeof(node));
                ptr = ptr->next;
                ptr->index = index;
                ptr->next = NULL;
            }
            ptr->val = val;
        }
        val_node* tmp = list->next;
        free(list);
        list = tmp;
    }
}

size_t apply_mask_1(char* mask, size_t raw) {
    char bit_res[MASK_SIZE];
    for (size_t i = MASK_SIZE; i--;) {
        bit_res[i] = mask[i] == 'X' ? '0' + raw % 2 : mask[i];
        raw /= 2;
    }

    size_t result = 0;
    for (size_t i = 0; i < MASK_SIZE; ++i) {
        result *= 2;
        result += (bit_res[i] - '0');
    }

    return result;
}

val_node* apply_mask_2(char* mask, size_t raw) {
    size_t iterations = 1;
    char base[MASK_SIZE];
    for (size_t i = MASK_SIZE; i--;) {
        base[i] = mask[i] == '0' ? '0' + raw % 2 : mask[i];
        if (mask[i] == 'X')
            iterations *= 2;
        raw /= 2;
    }
    val_node* result = NULL;

    for (size_t i = 0; i < iterations; ++i) {
        val_node* tmp_node = malloc(sizeof(val_node));
        tmp_node->next = result;
        result = tmp_node;
        result->val = 0;

        size_t curr = i;
        for (size_t j = 0; j < MASK_SIZE; ++j) {
            result->val *= 2;
            if (base[j] == 'X') {
                result->val += curr % 2;
                curr /= 2;
            } else {
                result->val += base[j] - '0';
            }
        }
    }

    return result;
}

int day_14_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    char addr[4];
    char mask[MASK_SIZE + 1];
    node* map_1[BUCKETS];
    node* map_2[BUCKETS];

    for (size_t i = 0; i < BUCKETS; ++i) {
        map_1[i] = NULL;
        map_2[i] = NULL;
    }

    while (fscanf(reader, "%4c", addr) != EOF) {
        if (addr[1] == 'a') {
            fscanf(reader, " = %s\n", mask);
        } else {
            size_t index;
            size_t raw;
            fscanf(reader, "%zd%*c = %zd\n", &index, &raw);
            set_1(map_1, index, apply_mask_1(mask, raw));
            set_2(map_2, apply_mask_2(mask, index), raw);
        }
    }

    size_t sum_1 = 0;
    for (size_t i = 0; i < BUCKETS; ++i)
        for (node* n = map_1[i]; n != NULL; n = n->next)
            sum_1 += n->val;
    printf("%zd\n", sum_1);

    size_t sum_2 = 0;
    for (size_t i = 0; i < BUCKETS; ++i)
        for (node* n = map_2[i]; n != NULL; n = n->next)
            sum_2 += n->val;
    printf("%zd\n", sum_2);

    return 0;
}

#endif // DAY_14

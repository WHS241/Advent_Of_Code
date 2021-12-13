#ifndef INTCODE_C
#define INTCODE_C

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t TAPE_SIZE = 10000;

typedef struct {
    size_t index;
    long value;
    void* next;
} cell;

cell* get_cell(cell** tape, size_t index) {
    size_t pos = index % TAPE_SIZE;
    cell* c = tape[pos];
    while (c && c->index != index && c->next)
        c = c->next;
    return c;
}

long get(cell** tape, size_t index) {
    cell* v = get_cell(tape, index);
    return (v == NULL || v->index != index) ? 0 : v->value;
}

void set(cell** tape, size_t index, long value) {
    cell* v = get_cell(tape, index);
    if (v && v->index == index) {
        v->value = value;
    } else {
        if (v) {
            assert(!(v->next));
            v->next = malloc(sizeof(cell));
            v = v->next;
        } else {
            // Null list at element in array
            size_t pos = index % TAPE_SIZE;
            v = tape[pos] = malloc(sizeof(cell));
        }
        v->index = index;
        v->value = value;
        v->next = NULL;
    }
}

cell** initialize(FILE* reader) {
    cell** tape = malloc(TAPE_SIZE * sizeof(cell*));
    for (size_t i = 0; i < TAPE_SIZE; ++i)
        tape[i] = NULL;

    size_t i = 0;
    long curr;
    while (fscanf(reader, "%ld", &curr) != EOF) {
        set(tape, i++, curr);
        fgetc(reader);
    }

    return tape;
}

void clear_cell(cell* c) {
    if (c) {
        clear_cell(c->next);
    }
    free(c);
}

void clear_tape(cell** tape) {
    for (size_t i = 0; i < TAPE_SIZE; ++i)
        clear_cell(tape[i]);
    free(tape);
}

void run(cell** tape) {
    size_t i = 0;
    while (true) {
        long opcode = get(tape, i);
        size_t step;
        switch (opcode) {
        case 1:
            {
                long arg1_pos = get(tape, i + 1);
                long arg2_pos = get(tape, i + 2);
                long out_pos = get(tape, i + 3);
                set(tape, out_pos, get(tape, arg1_pos) + get(tape, arg2_pos));
                step = 4;
            }
            break;

        case 2:
            {
                long arg1_pos = get(tape, i + 1);
                long arg2_pos = get(tape, i + 2);
                long out_pos = get(tape, i + 3);
                set(tape, out_pos, get(tape, arg1_pos) * get(tape, arg2_pos));
                step = 4;
            }
            break;

        case 99:
            return;
        }
        i += step;
    }
}

#endif // INTCODE_C

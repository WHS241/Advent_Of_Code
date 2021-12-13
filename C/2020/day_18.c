#ifndef DAY_18
#define DAY_18 2

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#if DAy_18 == 1
typedef struct {
    size_t curr;
    char op;
    void* next;
} part_1_node;
#elif DAY_18 == 2
typedef struct {
    size_t curr;
    void* next;
} val_node;

typedef struct {
    char op;
    void* next;
} op_node;

void clean_ops(val_node** val_stack, op_node** op_stack) {
    while (*val_stack && *op_stack && (*op_stack)->op != '(') {
        size_t op1 = (*val_stack)->curr;
        void* tmp = *val_stack;
        *val_stack = (*val_stack)->next;
        free(tmp);

        switch ((*op_stack)->op) {
        case '+':
            (*val_stack)->curr += op1;
            break;

        case '*':
            (*val_stack)->curr *= op1;
            break;

        default:
            break;
        }

        tmp = *op_stack;
        *op_stack = (*op_stack)->next;
        free(tmp);
    }
}

int get_precedence(char op) {
    if (op == '*')
        return 1;
    if (op == '+')
        return 2;
    return 0;
}
#endif

int day_18_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    size_t sum = 0;

#if DAY_18 == 1
    part_1_node* stack = NULL;
    size_t curr;
    char op = 0;
#elif DAY_18 == 2
    val_node* val_stack = NULL;
    op_node* op_stack = NULL;
#endif

    int c;
    while ((c = fgetc(reader)) != EOF) {
#if DAY_18 == 1
        int process_int = 0;
        if (c == '\n') {
            sum += curr;
            curr = 0;
            op = 0;
        } else if (isspace(c)) {
            // nop
        } else if (c == '(') {
            part_1_node* tmp = malloc(sizeof(part_1_node));
            tmp->curr = curr;
            tmp->op = op;
            tmp->next = stack;

            stack = tmp;

            op = 0;
        } else if (c == ')') {
            c = curr;
            curr = stack->curr;
            op = stack->op;
            part_1_node* tmp = stack;
            stack = stack->next;
            free(tmp);
            process_int = 1;
        } else if (c == '+' || c == '*') {
            op = c;
        } else {
            c -= '0';
            process_int = 1;
        }

        if (process_int) {
            switch (op) {
            case 0:
                curr = c;
                break;

            case '+':
                curr += c;
                op = 0;
                break;

            case '*':
                curr *= c;
                op = 0;
                break;

            default:
                break;
            }
        }
#elif DAY_18 == 2
        // Modified shunting-yard
        if (c == '\n') {
            clean_ops(&val_stack, &op_stack);
            val_node* tmp = val_stack;
            val_stack = val_stack->next;
            sum += tmp->curr;
            free(tmp);
        } else if (isspace(c)) {
            // nop
        } else if (isdigit(c)) {
            val_node* tmp = malloc(sizeof(val_node));
            tmp->next = val_stack;
            tmp->curr = c - '0';
            val_stack = tmp;
        } else if (c == ')') {
            clean_ops(&val_stack, &op_stack);
            void* tmp = op_stack;
            op_stack = op_stack->next;
            free(tmp);
        } else {
            if (c != '(') {
                while (val_stack && op_stack && get_precedence(c) <= get_precedence(op_stack->op)) {
                    size_t arg = val_stack->curr;
                    void* tmp = val_stack;
                    val_stack = val_stack->next;
                    free(tmp);

                    switch (op_stack->op) {
                    case '+':
                        val_stack->curr += arg;
                        break;

                    case '*':
                        val_stack->curr *= arg;
                        break;

                    default:
                        break;
                    }
                    tmp = op_stack;
                    op_stack = op_stack->next;
                    free(tmp);
                }
            }

            op_node* tmp = malloc(sizeof(op_node));
            tmp->op = c;
            tmp->next = op_stack;
            op_stack = tmp;
        }
#endif
    }

    printf("%zd\n", sum);

    return 0;
}

#endif // DAY_18

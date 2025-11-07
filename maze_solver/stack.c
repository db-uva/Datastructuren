#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct stack {
    int *stack;
    int capacity;
    int top;
    int max_elements;
    int n_pushes;
    int n_pops;
};

struct stack *stack_init(size_t capacity) {
    struct stack *s = malloc(sizeof(struct stack));
    if(s == NULL) {
        return NULL;
    }
    s->stack = malloc(capacity * sizeof(int));
    if (s->stack == NULL) {
        free(s);
        return NULL;
    }
    s->capacity = (int)capacity;
    s->top = 0;
    s->max_elements = 0;
    s->n_pops = 0;
    s->n_pushes = 0;

    return s;
}

void stack_cleanup(struct stack *s) {
    if(s == NULL) {
        return;
    }
    free(s->stack);
    free(s);
}

void stack_stats(const struct stack *s) {
    if(s == NULL) {
        return;
    }
    fprintf(stderr,"stats %d %d %d\n", s->n_pushes, s->n_pops, s->max_elements);
}

int stack_push(struct stack *s, int c) {
    if(s == NULL || s->top >= s->capacity) {
        return 1;
    }

    s->stack[s->top++] = c;
    s->n_pushes++;
    if (s->top > s->max_elements) {
        s->max_elements = s->top;
    }
    return 0;
}

int stack_pop(struct stack *s) {
    if(s == NULL || s->top == 0) {
        return -1;
    }
    s->top--;
    s->n_pops++;
    return s->stack[s->top];
}

int stack_peek(const struct stack *s) {
    if(s == NULL || s->top == 0) {
        return -1;
    }
    return s->stack[s->top - 1];
}

int stack_empty(const struct stack *s) {
    if(s == NULL) {
        return -1;
    } else if(s->top == 0) {
        return 1;
    } else {
        return 0;
    }
}

size_t stack_size(const struct stack *s) {
    if(s == NULL) {
        return 1;
    }
    return (size_t)s->top;
}

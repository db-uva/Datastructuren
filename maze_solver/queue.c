#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
    int *queue;
    int capacity;
    int head;
    int tail;
    int max_elements;
    int n_pushes;
    int n_pops;
};

struct queue *queue_init(size_t capacity) {
    struct queue *q = malloc(sizeof(struct queue));
    if(q == NULL) {
        return NULL;
    }
    q->queue = malloc(capacity * sizeof(int));
    if (q->queue == NULL) {
        free(q);
        return NULL;
    }
    q->capacity = (int)capacity;
    q->head = 0;
    q->tail = 0;
    q->max_elements = 0;
    q->n_pops = 0;
    q->n_pushes = 0;

    return q;
}

void queue_cleanup(struct queue *q) {
    if(q == NULL) {
        return;
    }
    free(q->queue);
    free(q);
}

void queue_stats(const struct queue *q) {
    if(q == NULL) {
        return;
    }
    fprintf(stderr,"stats %d %d %d\n", q->n_pushes, q->n_pops, q->max_elements);
}

int queue_push(struct queue *q, int e) {
    if(q == NULL || q->head >= q->capacity) {
        return 1;
    }

    q->queue[q->head++] = e;
    q->n_pushes++;
    if ((q->head - q->tail) > q->max_elements) {
        q->max_elements = q->head;
    }
    return 0;
}

int queue_pop(struct queue *q) {
    if(q == NULL || q->head == q->tail) {
        return -1;
    }
    q->n_pops++;
    return q->queue[q->tail++];
}

int queue_peek(const struct queue *q) {
    if(q == NULL || q->tail == q->head) {
        return -1;
    }
    return q->queue[q->tail];
}

int queue_empty(const struct queue *q) {
    if(q == NULL) {
        return -1;
    } else if(q->head == q->tail) {
        return 1;
    } else {
        return 0;
    }
}

size_t queue_size(const struct queue *q) {
    if(q == NULL) {
        return 1;
    }
    return (size_t)(q->head - q->tail);
}

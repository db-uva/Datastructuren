#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define NOT_FOUND -1
#define ERROR -2

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occurred.
 */
int bfs_solve(struct maze *m) {
    if (m == NULL) {
        return ERROR;
    }

    int size = maze_size(m);
    int capacity = 4000 > size * size ? 4000 : size * size;
    struct queue *q = queue_init((size_t)capacity);
    if (q == NULL) {
        return ERROR;
    }

    int *parent = malloc((size_t)capacity * sizeof(int));
    if (!parent) {
        queue_cleanup(q);
        return ERROR;
    }
    for (int i = 0; i < capacity; i++) {
        parent[i] = -1;
    }

    int r, c;
    maze_start(m, &r, &c);
    int start_index = maze_index(m, r, c);
    if (queue_push(q, start_index) != 0) {
        queue_cleanup(q);
        free(parent);
        return ERROR;
    }

    bool found = false;
    int dest_index = -1;

    while (!queue_empty(q)) {
        int index = queue_pop(q);
        int r = maze_row(m, index);
        int c = maze_col(m, index);

        if (maze_at_destination(m, r, c)) {
            found = true;
            dest_index = index;
            break;
        }

        if (!maze_at_start(m, r, c)) {
            maze_set(m, r, c, VISITED);
        }

        for (int i = 0; i < N_MOVES; i++) {
            int nr = r + m_offsets[i][0];
            int nc = c + m_offsets[i][1];

            if (!maze_valid_move(m, nr, nc)) {
                continue;
            }

            char cell = maze_get(m, nr, nc);
            if (cell == FLOOR || maze_at_destination(m, nr, nc)) {
                int next_index = maze_index(m, nr, nc);
                if (parent[next_index] == -1) {
                    parent[next_index] = index;
                    if (!maze_at_destination(m, nr, nc))
                        maze_set(m, nr, nc, TO_VISIT);
                    if (queue_push(q, next_index) != 0) {
                        queue_cleanup(q);
                        free(parent);
                        return ERROR;
                    }
                }
            }
        }
    }

    int path_length = 0;

    if (found) {
        int index = dest_index;
        while (index != -1 && index != start_index) {
            int r = maze_row(m, index);
            int c = maze_col(m, index);
            if (!maze_at_start(m, r, c) && !maze_at_destination(m, r, c)) {
                maze_set(m, r, c, PATH);
            }
            index = parent[index];
            path_length++;
        }
    } else {
        path_length = NOT_FOUND;
    }

    queue_stats(q);
    queue_cleanup(q);
    free(parent);

    if (found) {
        return path_length;
    } else {
        return NOT_FOUND;
    }
}

int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = bfs_solve(m);
    if (path_length == ERROR) {
        printf("bfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("bfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2

bool find_path(struct maze *m, struct stack *s) {
    bool found = false;
    while(!stack_empty(s)) {
        int index = stack_pop(s);
        int r = maze_row(m, index);
        int c = maze_col(m, index);

        if(maze_at_destination(m, r, c)) {
            found = true;
            break;
        }

        if(!maze_at_start(m, r, c)) {
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
                if (!maze_at_destination(m, nr, nc)) {
                    maze_set(m, nr, nc, TO_VISIT);
                }

                int next_index = maze_index(m, nr, nc);
                if (stack_push(s, next_index) != 0) {
                    stack_cleanup(s);
                    return ERROR;
                }
            }
        }
    }
    return found;
}

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m) {
    if (m == NULL) {
        return ERROR;
    }

    int size = maze_size(m);
    int capacity = 4000 > size * size ? 4000 : size * size;
    struct stack *s = stack_init((size_t)capacity);
    if (s == NULL) {
        return ERROR;
    }

    int r, c;
    maze_start(m, &r, &c);
    int index = maze_index(m, r, c);
    if (stack_push(s, index) != 0) {
        stack_cleanup(s);
        return ERROR;
    }

    bool found = find_path(m, s);

    int path_length = 0;

    if (found) {
        maze_destination(m, &r, &c);
        while (!maze_at_start(m, r, c)) {
            maze_set(m, r, c, PATH);
            path_length++;
            bool moved = false;
            for (int i = 0; i < N_MOVES && !moved; i++) {
                int nr = r + m_offsets[i][0];
                int nc = c + m_offsets[i][1];
                if (maze_valid_move(m, nr, nc) && maze_get(m, nr, nc) == VISITED) {
                    r = nr;
                    c = nc;
                    moved = true;
                }
            }
            if (!moved) {
                break;
            }
        }
    } else {
        path_length = NOT_FOUND;
    }

    stack_stats(s);
    stack_cleanup(s);

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
    int path_length = dfs_solve(m);
    if (path_length == ERROR) {
        printf("dfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("dfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
